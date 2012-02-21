Public Class SoundTable
    Inherits AutoIncrementTable

    Public Sub New(ByVal table As DataTable, ByVal database As XmlDB)
        MyBase.New(table, database)
    End Sub


    Public Overrides Sub LoadData()

        Dim fileName As String = GetStringProperty(tbl, TableProperty.FileName)
        Dim filePath As String = XmlDB.BaseDirectory & fileName
        If (XmlDB.IsLanguageSpecificFile(fileName)) Then
            filePath = XmlDB.GetLanguageSpecificBaseDirectory(fileName) & fileName
        End If

        tbl.Clear()
        Dim xr As New Xml.XmlTextReader(filePath)
        While xr.Read
            If xr.NodeType = Xml.XmlNodeType.Text Then
                Dim r As DataRow = tbl.NewRow
                r(Tables.LookupTableFields.Name) = xr.Value
                tbl.Rows.Add(r)
            End If
        End While
        xr.Close()
    End Sub

    Public Overrides Sub SaveData()

        Dim fileName As String = GetStringProperty(tbl, TableProperty.FileName)
        Dim filePath As String = XmlDB.BaseDirectory & fileName

        If XmlDB.IsLanguageSpecificFile(fileName) Then
            filePath = XmlDB.GetLanguageSpecificBaseDirectory(fileName) & fileName
        End If

        Dim xw As New Xml.XmlTextWriter(filePath, Text.Encoding.UTF8)
        xw.WriteStartElement(GetStringProperty(tbl, TableProperty.DataSetName))
        xw.WriteString(vbLf)
        For Each r As DataRow In tbl.Rows
            xw.WriteString(vbTab)
            xw.WriteElementString(Tables.Sounds, r(Tables.LookupTableFields.Name))
            xw.WriteString(vbLf)
        Next
        xw.WriteEndElement()
        xw.WriteString(vbLf)
        xw.Close()
        tbl.AcceptChanges()
    End Sub
End Class
