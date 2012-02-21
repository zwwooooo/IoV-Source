Module GridUtility
    Friend Const BlankLookupRowName As String = " "
    Friend Const BlankLookupRowValue As Integer = 0
    Friend Sub InitializeGrid(ByVal grid As DataGridView, ByVal view As DataView, Optional ByVal subTable As String = Nothing, Optional ByVal autoSizeColumns As Boolean = False)
        Dim viewCache As New Hashtable

        grid.AutoGenerateColumns = False
        grid.DataSource = view
        If grid.Columns.Count > 0 Then
            grid.Columns.Clear()
        End If

        AddHandler grid.DataError, AddressOf Grid_DataError

        For Each c As DataColumn In view.Table.Columns
            Application.DoEvents()
            Dim visible As Boolean = Not GetBooleanProperty(c, ColumnProperty.Grid_Hidden)
            Dim lookupTable As String = GetStringProperty(c, ColumnProperty.Lookup_Table)

            If GetBooleanProperty(c, ColumnProperty.SubTable) Then
                If subTable Is Nothing OrElse subTable.Length = 0 OrElse Not c.ColumnName.Contains(subTable) Then
                    visible = False
                End If
            End If

            If visible OrElse c Is view.Table.PrimaryKey(0) Then
                Dim dc As DataGridViewColumn

                If c.DataType Is GetType(Boolean) Then
                    dc = New DataGridViewCheckBoxColumn
                ElseIf c.DataType Is GetType(Image) Then
                    dc = New DataGridViewImageColumn
                ElseIf lookupTable IsNot Nothing Then
                    dc = New DataGridViewComboBoxColumn
                Else
                    dc = New DataGridViewTextBoxColumn
                End If

                With dc
                    .Name = c.ColumnName
                    .HeaderText = c.Caption
                    .DataPropertyName = c.ColumnName
                    .Resizable = DataGridViewTriState.True
                    .Visible = visible 'needed for primary key columns
                    .SortMode = DataGridViewColumnSortMode.Automatic

                    If autoSizeColumns Then
                        ' RoWa21: Removed that, so you can resize columns!
                        '.AutoSizeMode = DataGridViewAutoSizeColumnMode.DisplayedCells
                    Else
                        .AutoSizeMode = DataGridViewAutoSizeColumnMode.None
                        .Width = GetColumnWidth(c, subTable)
                    End If
                    If dc.GetType Is GetType(DataGridViewImageColumn) Then .AutoSizeMode = DataGridViewAutoSizeColumnMode.AllCells

                    .ToolTipText = c.ColumnName
                    Dim ttText = GetStringProperty(c, ColumnProperty.ToolTip)
                    If ttText IsNot Nothing Then
                        .ToolTipText &= vbCrLf & vbCrLf & ttText
                    End If

                    If lookupTable IsNot Nothing Then
                        Dim lookupValueColumn As String = GetStringProperty(c, ColumnProperty.Lookup_ValueColumn)
                        Dim lookupTextColumn As String = GetStringProperty(c, ColumnProperty.Lookup_TextColumn)
                        Dim lookupFilter As String = GetStringProperty(c, ColumnProperty.Lookup_Filter)
                        Dim lookupAddBlank As Boolean = GetBooleanProperty(c, ColumnProperty.Lookup_AddBlank)

                        With CType(dc, DataGridViewComboBoxColumn)
                            .ValueMember = lookupValueColumn
                            .DisplayMember = lookupTextColumn
                            Dim dv As DataView
                            Dim viewKey As String = view.Table.DataSet.Tables(lookupTable).TableName & lookupFilter & lookupTextColumn
                            If Not viewCache.ContainsKey(viewKey) Then
                                dv = New DataView(view.Table.DataSet.Tables(lookupTable), lookupFilter, lookupTextColumn, DataViewRowState.CurrentRows)
                                viewCache.Add(viewKey, dv)
                            Else
                                dv = CType(viewCache(viewKey), DataView)
                            End If
                            If Not lookupAddBlank Then
                                .DataSource = dv
                            Else
                                'this isn't used often enough to bother optimizing it
                                Dim dt As DataTable = dv.Table.Clone
                                Dim r As DataRow = dt.NewRow
                                r(Tables.LookupTableFields.ID) = BlankLookupRowValue
                                r(Tables.LookupTableFields.Name) = BlankLookupRowName
                                dt.Rows.Add(r)
                                For Each dvr As DataRowView In dv
                                    dt.Rows.Add(dvr.Row.ItemArray)
                                Next
                                .DataSource = dt
                            End If
                            If grid.ReadOnly Then
                                .DisplayStyle = DataGridViewComboBoxDisplayStyle.Nothing
                            Else
                                .DisplayStyle = DataGridViewComboBoxDisplayStyle.ComboBox
                            End If
                            .AutoComplete = True
                        End With
                    End If
                End With
                grid.Columns.Add(dc)
            End If
        Next
    End Sub

    Friend Sub DeleteGridRow(ByVal grid As DataGridView, ByVal e As System.Windows.Forms.DataGridViewRowCancelEventArgs)
        If grid.SelectionMode = DataGridViewSelectionMode.FullRowSelect AndAlso Not e.Row.IsNewRow Then
            Dim resp As DialogResult = MessageBox.Show("Are you sure you want to delete row #" & e.Row.Index + 1 & "?", "Delete Row", MessageBoxButtons.YesNo, MessageBoxIcon.Question, MessageBoxDefaultButton.Button2)
            If resp = Windows.Forms.DialogResult.No Then
                e.Cancel = True
            End If
        End If
    End Sub

    Friend Sub Grid_DataError(ByVal sender As Object, ByVal e As System.Windows.Forms.DataGridViewDataErrorEventArgs)
        If e.Exception Is Nothing Then
            'do nothing
        ElseIf e.Exception.GetType.Equals(GetType(ArgumentException)) Then
            e.Cancel = True 'lame I know, but it makes the comboboxes work
        ElseIf e.Exception.GetType.Equals(GetType(ConstraintException)) Then
            ErrorHandler.ShowError("This value has already been used. Please enter a different value.", MessageBoxIcon.Exclamation)
        Else
            ErrorHandler.ShowError(e.Exception.Message, MessageBoxIcon.Error)
        End If
    End Sub

    Friend Function GetActualGridHeight(ByVal grid As DataGridView) As Integer
        Dim height As Integer = grid.RowCount * grid.RowTemplate.Height
        height += grid.ColumnHeadersHeight + 30
        Return height
    End Function

    Friend Function GetActualGridWidth(ByVal grid As DataGridView) As Integer
        Dim width As Integer = grid.RowHeadersWidth + 10
        For Each c As DataGridViewColumn In grid.Columns
            If c.Visible Then width += c.Width
        Next
        Return width
    End Function

    Friend Sub SetColumnWidth(ByVal dc As DataGridViewColumn, ByVal table As DataTable, Optional ByVal subTable As String = Nothing)
        If table.Columns.Contains(dc.Name) Then
            Dim c As DataColumn = table.Columns(dc.Name)
            If GetBooleanProperty(c, ColumnProperty.SubTable) Then
                SetProperty(DB.Table(subTable).Columns(c.ColumnName.Remove(0, subTable.Length)), ColumnProperty.Width, dc.Width)
            Else
                SetProperty(c, ColumnProperty.Width, dc.Width)
            End If
        End If
    End Sub

    Friend Function GetColumnWidth(ByVal c As DataColumn, Optional ByVal subTable As String = Nothing) As Integer
        Dim width As String = Nothing
        If GetBooleanProperty(c, ColumnProperty.SubTable) Then
            width = GetStringProperty(DB.Table(subTable).Columns(c.ColumnName.Remove(0, subTable.Length)), ColumnProperty.Width)
        Else
            width = GetStringProperty(c, ColumnProperty.Width)
        End If
        If width IsNot Nothing Then
            Return CInt(width)
        Else
            Return 0
        End If
    End Function


End Module
