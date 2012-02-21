Public Class MercGearGridForm
    Inherits GridForm
    Protected Const ViewText As String = "&View Details"

    Public Sub New(ByVal formText As String, ByVal view As DataView)
        MyBase.New(formText, view)

        ' This call is required by the Windows Form Designer.
        InitializeComponent()

        ' Add any initialization after the InitializeComponent() call.
        Me.Text = formText
        With Me.Grid
            .ReadOnly = True
            .SelectionMode = DataGridViewSelectionMode.FullRowSelect
            .RowHeadersVisible = True
        End With

        GridContextMenu.Items.Add(ViewText, Nothing, AddressOf ViewMenuItem_Click)
    End Sub

    Private Sub ViewMenuItem_Click(ByVal sender As System.Object, ByVal e As System.EventArgs)
        If Grid.SelectedRows.Count > 0 Then
            MercGearDataForm.Open(Grid.SelectedRows(0).Cells(Tables.MercStartingGear.Fields.ID).Value, Grid.SelectedRows(0).Cells(Tables.MercStartingGear.Fields.Name).Value)
        End If
    End Sub

    Protected Sub Grid_CellMouseDoubleClick(ByVal sender As Object, ByVal e As System.Windows.Forms.DataGridViewCellMouseEventArgs) Handles Grid.CellMouseDoubleClick
        If e.RowIndex <> -1 Then
            MercGearDataForm.Open(Grid.Rows(e.RowIndex).Cells(Tables.MercStartingGear.Fields.ID).Value, Grid.Rows(e.RowIndex).Cells(Tables.MercStartingGear.Fields.Name).Value)
        End If
    End Sub
End Class