Public Class EnemyItemTable
    Inherits DefaultTable
    Const AmmoIndex As Integer = 19 'the key of the ammo row
    Protected ammoTable As DataTable 'need a 2nd table to store the ammo data

    Public Sub New(ByVal table As DataTable, ByVal manager As DataManager)
        MyBase.New(table, manager)
    End Sub

    Protected Sub AfterLoadAll(sender As DataManager) Handles _dm.AfterLoadData, _dm.AfterSaveData, _dm.AfterLoadWorkingData, _dm.AfterSaveWorkingData
        ammoTable = _table.DataSet.Tables(Tables.EnemyAmmo)
        Dim ammoRow As DataRow = _table.Rows.Find(AmmoIndex)
        If ammoRow IsNot Nothing Then
            Dim row As DataRow = ammoTable.NewRow
            row.ItemArray = ammoRow.ItemArray
            ammoTable.Rows.Add(row)
            _table.Rows.Remove(ammoRow)
        End If
    End Sub

    Protected Sub BeforeSaveAll(sender As DataManager) Handles _dm.BeforeSaveData, _dm.BeforeSaveWorkingData
        If ammoTable.Rows.Count > 0 Then
            Dim row As DataRow = _table.NewRow
            row.ItemArray = ammoTable.Rows(0).ItemArray
            _table.Rows.Add(row)
            ammoTable.Rows.RemoveAt(0)
        End If
    End Sub
End Class
