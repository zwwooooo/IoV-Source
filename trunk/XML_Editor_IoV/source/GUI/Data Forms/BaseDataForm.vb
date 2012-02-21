Public Class BaseDataForm
    Protected view As DataView
    Protected id As Integer

    Public Sub New()

        ' This call is required by the Windows Form Designer.
        InitializeComponent()

        ' Add any initialization after the InitializeComponent() call.

    End Sub

    Public Sub New(ByVal recordID As Integer, ByVal formText As String)
        LoadingForm.Show()
        Application.DoEvents()
        ' This call is required by the Windows Form Designer.
        InitializeComponent()

        ' Add any initialization after the InitializeComponent() call.
        Me.AcceptButton = OKButton
        Me.Text = formText
        id = recordID
    End Sub

#Region " Data Binding "
    Protected Sub Bind(ByVal tableName As String, ByVal filter As String)
        view = New DataView(DB.Table(tableName), filter, "", DataViewRowState.CurrentRows)
        view(0).BeginEdit()
        BindControls(CType(Me, Control))
    End Sub

    Protected Sub BindControls(ByVal parent As Control)
        For Each ctl As Control In parent.Controls
            If ctl.Tag IsNot Nothing Then
                If TypeOf ctl Is TextBox OrElse TypeOf ctl Is Label OrElse TypeOf ctl Is MaskedTextBox Then
                    ctl.DataBindings.Add("Text", view, ctl.Tag, True, DataSourceUpdateMode.OnPropertyChanged)
                ElseIf TypeOf ctl Is CheckBox OrElse TypeOf ctl Is RadioButton Then
                    ctl.DataBindings.Add("Checked", view, ctl.Tag, True, DataSourceUpdateMode.OnPropertyChanged)
                ElseIf TypeOf ctl Is NumericUpDown Then
                    ctl.DataBindings.Add("Value", view, ctl.Tag, True, DataSourceUpdateMode.OnPropertyChanged)
                ElseIf TypeOf ctl Is PictureBox Then
                    ctl.DataBindings.Add("Image", view, ctl.Tag, True, DataSourceUpdateMode.OnPropertyChanged)
                ElseIf TypeOf ctl Is ComboBox Then
                    ctl.DataBindings.Add("SelectedValue", view, ctl.Tag, True, DataSourceUpdateMode.OnPropertyChanged)

                    Dim lookupTable As String = GetStringProperty(view.Table.Columns(ctl.Tag), ColumnProperty.Lookup_Table)
                    If lookupTable IsNot Nothing Then
                        Dim lookupTextField As String = GetStringProperty(view.Table.Columns(ctl.Tag), ColumnProperty.Lookup_TextColumn)
                        Dim lookupValueField As String = GetStringProperty(view.Table.Columns(ctl.Tag), ColumnProperty.Lookup_ValueColumn)
                        Dim lookupFilter As String = GetStringProperty(view.Table.Columns(ctl.Tag), ColumnProperty.Lookup_Filter)
                        Dim lookupSort As String = GetStringProperty(view.Table.Columns(ctl.Tag), ColumnProperty.Lookup_Sort)
                        If lookupSort Is Nothing Then lookupSort = lookupTextField

                        Dim cbo As ComboBox = DirectCast(ctl, ComboBox)
                        cbo.ValueMember = lookupValueField
                        cbo.DisplayMember = lookupTextField
                        cbo.DataSource = New DataView(DB.Table(lookupTable), lookupFilter, lookupSort, DataViewRowState.CurrentRows)
                    End If
                End If

                Dim tooltip As String = GetStringProperty(view.Table.Columns(ctl.Tag), ColumnProperty.ToolTip)
                If tooltip IsNot Nothing Then DataToolTip.SetToolTip(ctl, tooltip)
            End If

            If ctl.Controls.Count > 0 Then
                BindControls(ctl)
            End If
        Next
    End Sub

    Protected Overridable Function CommitData() As Boolean
        view(0).EndEdit()
        view(0).Row.AcceptChanges()
        Return True
    End Function

    Protected Overridable Sub DoCancelData()
    End Sub

    Protected Sub CancelData()
        If view IsNot Nothing Then
            If view.Count > 0 Then view(0).CancelEdit()
            DoCancelData()
            view.Dispose()
        End If
    End Sub

#End Region

#Region " Buttons "

    Protected Overridable Sub ApplyButtonClicked()
    End Sub

    Private Sub ApplyButton_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles ApplyButton.Click
        If CommitData() Then
            ApplyButtonClicked()
            view(0).BeginEdit()
        End If
    End Sub

    Protected Overridable Sub OKButtonClicked()
    End Sub

    Private Sub OKButton_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles OKButton.Click
        If CommitData() Then
            OKButtonClicked()
            Close()
            view.Dispose()
        End If
    End Sub

    Protected Overridable Sub CancelButtonClicked()
    End Sub

    Private Sub CancelButton_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles CancelButton1.Click
        CancelButtonClicked()
        CancelData()
        Close()
    End Sub

    Private Sub DataForm_FormClosing(ByVal sender As Object, ByVal e As System.Windows.Forms.FormClosingEventArgs) Handles Me.FormClosing
        If e.CloseReason = CloseReason.UserClosing AndAlso Me.DialogResult <> Windows.Forms.DialogResult.OK Then
            CancelButtonClicked()
            CancelData()
        End If
    End Sub

#End Region


#Region " Status Bar "
    Private Sub DataForm_Activated(ByVal sender As Object, ByVal e As System.EventArgs) Handles Me.Activated
        UpdateStatusBar()
    End Sub

    Private Sub DataForm_Load(ByVal sender As Object, ByVal e As System.EventArgs) Handles Me.Load
        Me.WindowState = FormWindowState.Normal
        LoadingForm.Close()
    End Sub

    Protected Sub UpdateStatusBar()
        If MainWindow IsNot Nothing Then MainWindow.StatusLabel.Text = "Editing " & Me.Text
    End Sub

    Private Sub Form_VisibleChanged(ByVal sender As Object, ByVal e As System.EventArgs) Handles Me.VisibleChanged
        If MainWindow IsNot Nothing Then
            If Not Me.Visible Then MainWindow.StatusLabel.Text = ""
        End If
    End Sub

#End Region


End Class