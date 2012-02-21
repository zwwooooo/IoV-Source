Imports GUI.GUI

Public Class GridForm
    Protected view As DataView
    Protected subTable As String
    Protected origFilter As String
    Protected customFilter As String
    Private heightProperty As String
    Private widthProperty As String
    Private topProperty As String
    Private leftProperty As String
    Private stateProperty As String
    Private openedBeforeProperty As String

    Public Sub New(ByVal formText As String, ByVal vw As DataView, Optional ByVal subTable As String = Nothing)
        LoadingForm.Show()
        Application.DoEvents()
        ' This call is required by the Windows Form Designer.
        InitializeComponent()

        ' Add any initialization after the InitializeComponent() call.
        Me.Text = formText
        Dim spacelessText As String = Replace(Me.Text, " ", "_")
        heightProperty = spacelessText & "_Height"
        widthProperty = spacelessText & "_Width"
        topProperty = spacelessText & "_Top"
        leftProperty = spacelessText & "_Left"
        stateProperty = spacelessText & "_WindowState"
        openedBeforeProperty = spacelessText & "_OpenedBefore"

        view = vw
        origFilter = view.RowFilter
        Me.subTable = subTable
        Dim openedBefore As Boolean = CBool(SettingsUtility.GetSettingsValue(openedBeforeProperty, False))

        InitializeGrid(Grid, view, subTable, Not openedBefore)
    End Sub

    Public Property Filter() As String
        Get
            Return customFilter
        End Get
        Set(ByVal value As String)
            customFilter = Replace(value, Chr(34), "'")
            If customFilter IsNot Nothing AndAlso customFilter.Length > 0 Then
                Try
                    If origFilter IsNot Nothing AndAlso origFilter.Length > 0 Then
                        view.RowFilter = origFilter & " AND " & customFilter
                    Else
                        view.RowFilter = customFilter
                    End If
                Catch ex As Exception
                    ErrorHandler.ShowError("Invalid expression.  Check the tooltips in the column headings for the fieldnames.  A standard SQL expression is expected.", "Filter Error", ex)
                    view.RowFilter = origFilter
                End Try
            Else
                view.RowFilter = origFilter
            End If
        End Set
    End Property

    Private Sub Grid_SelectionChanged(ByVal sender As Object, ByVal e As System.EventArgs) Handles Grid.SelectionChanged
        UpdateStatusBar()
    End Sub

    Private Sub Grid_UserAddedRow(ByVal sender As Object, ByVal e As System.Windows.Forms.DataGridViewRowEventArgs) Handles Grid.UserAddedRow
        UpdateStatusBar()
    End Sub

    Private Sub Grid_UserDeletedRow(ByVal sender As Object, ByVal e As System.Windows.Forms.DataGridViewRowEventArgs) Handles Grid.UserDeletedRow
        UpdateStatusBar()
    End Sub

    Private Sub Grid_UserDeletingRow(ByVal sender As Object, ByVal e As System.Windows.Forms.DataGridViewRowCancelEventArgs) Handles Grid.UserDeletingRow
        DeleteGridRow(DirectCast(sender, DataGridView), e)
    End Sub

    Private Sub SelectColumnsToolStripMenuItem_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles SelectColumnsToolStripMenuItem.Click
        Dim curState As FormWindowState = Me.WindowState
        Dim frm As New ColumnSelectForm(Me.Grid, subTable)
        frm.ShowDialog(Me)
        Me.Hide()
        Me.WindowState = FormWindowState.Normal
        LoadingForm.Show()
        InitializeGrid(Grid, view, subTable)
        LoadingForm.Close()
        Me.Show()
        Me.WindowState = curState
    End Sub

    Private Sub GridForm_Activated(ByVal sender As Object, ByVal e As System.EventArgs) Handles Me.Activated
        UpdateStatusBar()
        Me.Refresh()
    End Sub

    Private Sub GridForm_FormClosing(ByVal sender As Object, ByVal e As System.Windows.Forms.FormClosingEventArgs) Handles Me.FormClosing
        SettingsUtility.SetSettingsValue(heightProperty, Me.Height)
        SettingsUtility.SetSettingsValue(widthProperty, Me.Width)
        SettingsUtility.SetSettingsValue(topProperty, Me.Top)
        SettingsUtility.SetSettingsValue(leftProperty, Me.Left)
        SettingsUtility.SetSettingsValue(stateProperty, Me.WindowState)
        SettingsUtility.SetSettingsValue(openedBeforeProperty, True)
        SaveColumnWidths()
    End Sub

    Private Sub GridForm_Load(ByVal sender As Object, ByVal e As System.EventArgs) Handles Me.Load
        LoadingForm.Close()

        Dim savedTop As Integer = SettingsUtility.GetSettingsValue(topProperty, 0)
        Dim savedLeft As Integer = SettingsUtility.GetSettingsValue(leftProperty, 0)
        If savedLeft > 0 Then
            Me.Left = savedLeft
        Else
            Me.Left = 0
        End If
        If savedTop > 0 Then
            Me.Top = savedTop
        Else
            Me.Top = 0
        End If

        Dim savedHeight As Integer = SettingsUtility.GetSettingsValue(heightProperty, 0)
        Dim savedWidth As Integer = SettingsUtility.GetSettingsValue(widthProperty, 0)
        If savedHeight = 0 Or savedWidth = 0 Then
            Me.Width = Math.Min(MainWindow.ClientRectangle.Width - 50, GetActualGridWidth(Grid))
            Me.Height = Math.Min(MainWindow.ClientRectangle.Height - 100, GetActualGridHeight(Grid))
        Else
            Me.Width = Math.Min(MainWindow.ClientRectangle.Width - Me.Left - 50, savedWidth)
            Me.Height = Math.Min(MainWindow.ClientRectangle.Height - Me.Top - 100, savedHeight)
        End If

        Dim savedState As FormWindowState = SettingsUtility.GetSettingsValue(stateProperty, 0)
        If savedState <> FormWindowState.Minimized Then Me.WindowState = savedState
    End Sub

    Private Sub FilterToolStripMenuItem_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles FilterToolStripMenuItem.Click
        Dim frm As New CustomFilterForm
        frm.Filter = Me.Filter
        If frm.ShowDialog(Me) = Windows.Forms.DialogResult.OK Then Me.Filter = frm.Filter
    End Sub

    Protected Sub UpdateStatusBar()
        MainWindow.StatusLabel.Text = Me.Text & ": "
        If Grid.SelectedCells.Count > 0 Then
            MainWindow.StatusLabel.Text &= Grid.SelectedCells(0).RowIndex + 1 & " of "
        End If
        MainWindow.StatusLabel.Text &= view.Count & " Rows"
    End Sub

    Private Sub Form_VisibleChanged(ByVal sender As Object, ByVal e As System.EventArgs) Handles Me.VisibleChanged
        If Not Me.Visible Then MainWindow.StatusLabel.Text = ""
    End Sub

    Private Sub SaveColumnWidths()
        For Each dc As DataGridViewColumn In Grid.Columns
            SetColumnWidth(dc, view.Table, subTable)
        Next
        DB.SaveSchema()
    End Sub
End Class