Imports System.Xml
Imports System.IO
Public Class DefaultTable
    Protected tbl As DataTable
    Protected WithEvents db As XmlDB

    Public Sub New(ByVal table As DataTable, ByVal database As XmlDB)
        tbl = table
        db = database
    End Sub

    Public ReadOnly Property Table() As DataTable
        Get
            Return tbl
        End Get
    End Property

    Public Overridable Sub LoadInternalData(ByVal itemTable As DataTable)
        Dim i As Integer

        For i = 0 To itemTable.Rows.Count - 1
            If itemTable.Rows(i).Item("usItemClass") = 256 Then
                tbl.Rows.Add()
                tbl.Rows(tbl.Rows.Count - 1).Item("uiIndex") = itemTable.Rows(i).Item("ubClassIndex")
                If tbl.Rows.Count - 1 = 0 Then
                    tbl.Rows(tbl.Rows.Count - 1).Item("szItemName") = "None/" & itemTable.Rows(i).Item("szItemName")
                Else
                    tbl.Rows(tbl.Rows.Count - 1).Item("szItemName") = itemTable.Rows(i).Item("szItemName")
                End If
            End If
        Next

    End Sub

    Public Overridable Sub LoadItemData(ByVal fileName As String, ByVal filePath As String)
        Dim xmldoc As New XmlDataDocument()
        Dim xmlnode As XmlNode
        Dim xmlnode2 As XmlNodeList
        Dim xmlnode3 As XmlNodeList
        Dim i As Integer
        Dim x As Integer
        Dim y As Integer
        Dim a As Integer
        Dim da As Integer
        Dim uicomments As Integer = 0
        Dim fs As New FileStream(filePath, FileMode.Open, FileAccess.Read)
        xmldoc.Load(fs)
        xmlnode = xmldoc.GetElementsByTagName("ITEMLIST").Item(0)
        For i = 0 To xmlnode.ChildNodes.Count - 1
            If xmlnode.ChildNodes.Item(i).Name = "#comment" Then
                uicomments = uicomments + 1
                Continue For
            End If
            tbl.Rows.Add()
            a = 0
            da = 0
            xmlnode2 = xmlnode.ChildNodes.Item(i).ChildNodes
            For x = 0 To xmlnode2.Count - 1
                If xmlnode2.Item(x).Name = "#comment" Then Continue For
                If xmlnode2.Item(x).Name = "STAND_MODIFIERS" OrElse xmlnode2.Item(x).Name = "CROUCH_MODIFIERS" OrElse xmlnode2.Item(x).Name = "PRONE_MODIFIERS" Then
                    If xmlnode2.Item(x).Name = "STAND_MODIFIERS" Then a = 1
                    If xmlnode2.Item(x).Name = "CROUCH_MODIFIERS" Then a = 2
                    If xmlnode2.Item(x).Name = "PRONE_MODIFIERS" Then a = 3
                    xmlnode3 = xmlnode2.Item(x).ChildNodes
                    For y = 0 To xmlnode3.Count - 1
                        If xmlnode3.Item(y).Name = "#comment" Then Continue For
                        If tbl.Columns(xmlnode3.Item(y).Name & a).DataType.Name = "Boolean" Then
                            tbl.Rows(i - uicomments).Item(xmlnode3.Item(y).Name & a) = IIf(xmlnode3.Item(y).InnerText.Trim = 1, True, False)
                        Else
                            tbl.Rows(i - uicomments).Item(xmlnode3.Item(y).Name & a) = xmlnode3.Item(y).InnerText.Trim
                        End If
                    Next
                Else
                    If xmlnode2.Item(x).Name = "DefaultAttachment" Then
                        If da < 20 Then
                            tbl.Rows(i - uicomments).Item(xmlnode2.Item(x).Name & da) = xmlnode2.Item(x).InnerText.Trim
                            da = da + 1
                        End If
                    Else
                        If tbl.Columns(xmlnode2.Item(x).Name).DataType.Name = "Boolean" Then
                            tbl.Rows(i - uicomments).Item(xmlnode2.Item(x).Name) = IIf(xmlnode2.Item(x).InnerText.Trim = 1, True, False)
                        Else
                            tbl.Rows(i - uicomments).Item(xmlnode2.Item(x).Name) = xmlnode2.Item(x).InnerText.Trim
                        End If
                    End If
                End If
            Next
        Next
    End Sub

    Public Overridable Sub LoadMSGData(ByVal fileName As String, ByVal filePath As String)
        Dim xmldoc As New XmlDataDocument()
        Dim xmlnode As XmlNode
        Dim xmlnode2 As XmlNodeList
        Dim xmlnode3 As XmlNodeList
        Dim i As Integer
        Dim x As Integer
        Dim y As Integer
        Dim a As Integer
        Dim uiComments As Integer = 0
        Dim fs As New FileStream(filePath, FileMode.Open, FileAccess.Read)
        xmldoc.Load(fs)
        xmlnode = xmldoc.GetElementsByTagName("MERCGEARLIST").Item(0)
        For i = 0 To xmlnode.ChildNodes.Count - 1
            If xmlnode.ChildNodes.Item(i).Name = "#comment" Then
                uiComments = uiComments + 1
                Continue For
            End If
            tbl.Rows.Add()
            a = 0
            xmlnode2 = xmlnode.ChildNodes.Item(i).ChildNodes
            For x = 0 To xmlnode2.Count - 1
                If xmlnode2.Item(x).Name = "#comment" Then Continue For
                If xmlnode2.Item(x).Name = "GEARKIT" Then
                    a = a + 1
                    xmlnode3 = xmlnode2.Item(x).ChildNodes
                    For y = 0 To xmlnode3.Count - 1
                        If xmlnode3.Item(y).Name = "#comment" Then Continue For
                        If tbl.Columns(xmlnode3.Item(y).Name & a).DataType.Name = "Boolean" Then
                            tbl.Rows(i - uiComments).Item(xmlnode3.Item(y).Name & a) = IIf(xmlnode3.Item(y).InnerText.Trim = 1, True, False)
                        Else
                            tbl.Rows(i - uiComments).Item(xmlnode3.Item(y).Name & a) = xmlnode3.Item(y).InnerText.Trim
                        End If
                    Next
                Else
                    If tbl.Columns(xmlnode2.Item(x).Name).DataType.Name = "Boolean" Then
                        tbl.Rows(i - uiComments).Item(xmlnode2.Item(x).Name) = IIf(xmlnode2.Item(x).InnerText.Trim = 1, True, False)
                    Else
                        tbl.Rows(i - uiComments).Item(xmlnode2.Item(x).Name) = xmlnode2.Item(x).InnerText.Trim
                    End If
                End If
            Next
        Next
    End Sub

    Public Overridable Sub LoadControlData(ByVal fileName As String, ByVal filePath As String)
        Dim xmldoc As New XmlDataDocument()
        Dim fs As New FileStream(filePath, FileMode.Open, FileAccess.Read)
        xmldoc.Load(fs)

        Dim xmlnode As XmlNode = xmldoc.GetElementsByTagName("INVENTORYLIST").Item(0)
        Dim xmlParentNode As XmlNodeList
        Dim xmlChildNode As XmlNodeList
        Dim xmlChild2Node As XmlNodeList
        For i As Integer = 0 To xmlnode.ChildNodes.Count - 1
            If xmlnode.ChildNodes.Item(i).Name = "INVENTORY" Then Continue For
            tbl.Rows.Add()
            xmlParentNode = xmlnode.ChildNodes.Item(i).ChildNodes
            For x As Integer = 0 To xmlParentNode.Count - 1
                If xmlParentNode.Item(x).Name = "#comment" Then Continue For
                If xmlParentNode.Item(x).Name = "REORDERDAYSDELAY" OrElse xmlParentNode.Item(x).Name = "CASH" OrElse xmlParentNode.Item(x).Name = "COOLNESS" OrElse xmlParentNode.Item(x).Name = "BASICDEALERFLAGS" Then
                    xmlChildNode = xmlParentNode.Item(x).ChildNodes
                    For y As Integer = 0 To xmlChildNode.Count - 1
                        If xmlChildNode.Item(y).Name = "#comment" Then Continue For
                        If xmlChildNode.Item(y).Name = "DAILY" Then
                            xmlChild2Node = xmlChildNode.Item(y).ChildNodes
                            For z As Integer = 0 To xmlChild2Node.Count - 1
                                If xmlChild2Node.Item(z).Name = "#comment" Then Continue For
                                If tbl.Columns(xmlChild2Node.Item(z).Name).DataType.Name = "Boolean" Then
                                    tbl.Rows(tbl.Rows.Count - 1).Item(xmlChild2Node.Item(z).Name) = IIf(xmlChild2Node.Item(z).InnerText.Trim = 1, True, False)
                                Else
                                    tbl.Rows(tbl.Rows.Count - 1).Item(xmlChild2Node.Item(z).Name) = xmlChild2Node.Item(z).InnerText.Trim
                                End If
                            Next
                        Else
                            If tbl.Columns(xmlChildNode.Item(y).Name).DataType.Name = "Boolean" Then
                                tbl.Rows(tbl.Rows.Count - 1).Item(xmlChildNode.Item(y).Name) = IIf(xmlChildNode.Item(y).InnerText.Trim = 1, True, False)
                            Else
                                tbl.Rows(tbl.Rows.Count - 1).Item(xmlChildNode.Item(y).Name) = xmlChildNode.Item(y).InnerText.Trim
                            End If
                        End If
                    Next
                Else
                    If tbl.Columns(xmlParentNode.Item(x).Name).DataType.Name = "Boolean" Then
                        tbl.Rows(tbl.Rows.Count - 1).Item(xmlParentNode.Item(x).Name) = IIf(xmlParentNode.Item(x).InnerText.Trim = 1, True, False)
                    Else
                        tbl.Rows(tbl.Rows.Count - 1).Item(xmlParentNode.Item(x).Name) = xmlParentNode.Item(x).InnerText.Trim
                    End If
                End If
            Next
        Next
    End Sub

    Public Overridable Sub LoadData()
        Const Temp As String = "temp"

        Dim filePath As String = ""

        Dim t As DataTable = Nothing
        tbl.BeginLoadData()
        tbl.Clear()
        Dim fileName As String = GetStringProperty(tbl, TableProperty.FileName)

        filePath = XmlDB.BaseDirectory & fileName
        If XmlDB.IsLanguageSpecificFile(fileName) Then
            filePath = XmlDB.GetLanguageSpecificBaseDirectory(fileName) & fileName
        End If

        Dim sourceName As String = GetStringProperty(tbl, TableProperty.SourceTableName)
        If sourceName Is Nothing Then
            If fileName = "Items.xml" Then
                LoadItemData(fileName, filePath)
            ElseIf fileName = "MercStartingGear.xml" Then
                LoadMSGData(fileName, filePath)
            Else
                tbl.ReadXml(filePath)
            End If
        Else
            Dim tableName As String = tbl.TableName
            For Each t In tbl.DataSet.Tables
                If t.TableName = sourceName Then
                    t.TableName = Temp
                    Exit For
                End If
            Next

            tbl.TableName = sourceName
            If tableName.EndsWith("Control") Then
                LoadControlData(fileName, filePath)
            Else
                tbl.ReadXml(filePath)
            End If
            tbl.TableName = tableName
            If t IsNot Nothing AndAlso t.TableName = Temp Then t.TableName = sourceName
        End If
        tbl.EndLoadData()
    End Sub

    Public Overridable Sub ClearData()
        tbl.Clear()
    End Sub

    Public Overridable Sub SaveData()
        SaveData(tbl)
    End Sub

    Protected Overridable Sub SaveData(ByVal table As DataTable)
        If Not table.ExtendedProperties.Contains(TableProperty.SourceTableName) Then

            Dim fileName = table.ExtendedProperties(TableProperty.FileName)
            Dim filePath As String = XmlDB.BaseDirectory & fileName

            If XmlDB.IsLanguageSpecificFile(fileName) Then
                filePath = XmlDB.GetLanguageSpecificBaseDirectory(fileName) & fileName
            End If

            WriteXml(table, filePath)
        Else
            Dim t As DataTable = table.Copy
            t.TableName = table.ExtendedProperties(TableProperty.SourceTableName)

            Dim fileName As String = GetStringProperty(t, TableProperty.FileName)
            Dim filePath As String = XmlDB.BaseDirectory & fileName

            If XmlDB.IsLanguageSpecificFile(fileName) Then
                filePath = XmlDB.GetLanguageSpecificBaseDirectory(fileName) & fileName
            End If

            WriteXml(t, filePath)
            t.Dispose()
        End If
    End Sub

    'trim property = true results in all 0/blank values not being written to xml, except for the first entry, which is preserved for reference
    Protected Sub WriteXml(ByVal table As DataTable, ByVal fileName As String)

        ' RoWa21:  Special case for MercStartingGear.xml
        If table.TableName = "MERCGEAR" Then
            WriteXml_MercStartingGear(table, fileName)
        ElseIf table.TableName = "ITEM" Then
            ' CHRISL: Special case for items.xml
            WriteXml_Items(table, fileName)
        ElseIf table.TableName = "ITEMTOEXPLOSIVE" OrElse table.TableName = "ShopkeeperNames" Then
            'Do nothing.  This is an internal table that we don't want to create an xml file for.
        ElseIf table.TableName = "CONTROL" Then
            WriteXml_Control(table, fileName)
        ElseIf table.TableName = "INVENTORY" Then
            WriteXml_Inventory(table, fileName)
        Else
            'the stupid table.WriteXml method doesn't let you sort the data first
            Dim view As New DataView(table, "", table.Columns(0).ColumnName, DataViewRowState.CurrentRows)
            Dim trim As Boolean = GetBooleanProperty(table, TableProperty.Trim)
            Dim sourceDSName = GetStringProperty(table, TableProperty.DataSetName)
            If sourceDSName Is Nothing Then
                If table.DataSet IsNot Nothing Then
                    sourceDSName = table.DataSet.DataSetName
                Else
                    sourceDSName = SchemaName
                End If
            End If

            Dim xw As New Xml.XmlTextWriter(fileName, Text.Encoding.UTF8)
            xw.WriteStartDocument()
            xw.WriteWhitespace(vbLf)
            xw.WriteStartElement(sourceDSName)
            xw.WriteWhitespace(vbLf)

            For i As Long = 0 To view.Count - 1
                xw.WriteString(vbTab)
                xw.WriteStartElement(table.TableName)
                xw.WriteString(vbLf)

                Dim dcIndex As Integer = -1

                For Each c As DataColumn In table.Columns
                    dcIndex = dcIndex + 1

                    If Not trim OrElse (i = 0 OrElse c Is table.PrimaryKey(0) OrElse ((c.DataType.Equals(GetType(Boolean)) OrElse c.DataType.Equals(GetType(Decimal)) OrElse c.DataType.Equals(GetType(Integer))) AndAlso view(i)(c.ColumnName) <> 0) _
                        OrElse (c.DataType.Equals(GetType(String)) AndAlso view(i)(c.ColumnName) <> "")) Then

                        xw.WriteString(vbTab)
                        xw.WriteString(vbTab)

                        If Not c.DataType.Equals(GetType(Boolean)) Then
                            xw.WriteElementString(c.ColumnName, view(i)(c.ColumnName))
                        Else
                            If view(i)(c.ColumnName) Then
                                xw.WriteElementString(c.ColumnName, 1)
                            Else
                                xw.WriteElementString(c.ColumnName, 0)
                            End If
                        End If

                        xw.WriteString(vbLf)
                    End If
                Next
                xw.WriteString(vbTab)
                xw.WriteEndElement()
                xw.WriteString(vbLf)
            Next
            xw.WriteEndElement()
            xw.Close()
            view.Dispose()

            table.AcceptChanges()

        End If
    End Sub

    Protected Sub WriteXml_Items(ByVal table As DataTable, ByVal fileName As String)
        Dim view As New DataView(table, "", table.Columns(0).ColumnName, DataViewRowState.CurrentRows)
        Dim xw As New XmlTextWriter(fileName, System.Text.Encoding.UTF8)
        Dim i As Integer
        Dim x As Integer
        Dim da As Integer
        Dim trim As Boolean = GetBooleanProperty(table, TableProperty.Trim)
        Dim indent As Boolean = True

        xw.WriteStartDocument()
        xw.WriteWhitespace(vbLf)
        xw.WriteStartElement(table.ExtendedProperties("DataSetName").ToString)
        xw.WriteWhitespace(vbLf)
        For i = 0 To view.Count - 1
            If Not table.Rows(i).RowState = DataRowState.Deleted Then
                indent = True
                da = 0
                xw.WriteString(vbTab)
                xw.WriteStartElement(table.TableName)
                xw.WriteString(vbLf)
                For x = 0 To table.Rows(i).ItemArray.Length - 1
                    Dim c As DataColumn = table.Columns(x)
                    If c.ColumnName.EndsWith("1") And Not c.ColumnName.StartsWith("DefaultAttachment") Then Exit For
                    If Not trim OrElse (i = 0 OrElse c Is table.PrimaryKey(0) OrElse ((c.DataType.Equals(GetType(Boolean)) OrElse c.DataType.Equals(GetType(Decimal)) OrElse c.DataType.Equals(GetType(Integer))) AndAlso view(i)(c.ColumnName) <> 0) _
                        OrElse (c.DataType.Equals(GetType(String)) AndAlso view(i)(c.ColumnName) <> "")) Then
                        xw.WriteString(vbTab)
                        xw.WriteString(vbTab)
                        If Not c.DataType.Equals(GetType(Boolean)) Then
                            If c.ColumnName.Contains("DefaultAttachment") Then
                                xw.WriteElementString("DefaultAttachment", view(i)(c.ColumnName))
                            Else
                                xw.WriteElementString(c.ColumnName, view(i)(c.ColumnName))
                            End If
                        Else
                            If view(i)(c.ColumnName) Then
                                xw.WriteElementString(c.ColumnName, 1)
                            Else
                                xw.WriteElementString(c.ColumnName, 0)
                            End If
                        End If
                        xw.WriteString(vbLf)
                    End If
                Next
                'Start STAND_MODIFIERS section
                xw.WriteString(vbTab)
                xw.WriteString(vbTab)
                xw.WriteStartElement("STAND_MODIFIERS")
                For x = x To table.Rows(i).ItemArray.Length - 1
                    Dim c As DataColumn = table.Columns(x)
                    If c.ColumnName.EndsWith("2") Then Exit For
                    If Not trim OrElse (i = 0 OrElse c Is table.PrimaryKey(0) OrElse ((c.DataType.Equals(GetType(Boolean)) OrElse c.DataType.Equals(GetType(Decimal)) OrElse c.DataType.Equals(GetType(Integer))) AndAlso view(i)(c.ColumnName) <> -101) _
                       OrElse (c.DataType.Equals(GetType(String)) AndAlso view(i)(c.ColumnName) <> "")) Then
                        If indent = True Then
                            xw.WriteString(vbLf)
                            indent = False
                        End If
                        xw.WriteString(vbTab)
                        xw.WriteString(vbTab)
                        xw.WriteString(vbTab)
                        If Not c.DataType.Equals(GetType(Boolean)) Then
                            xw.WriteElementString(c.ColumnName.Remove(c.ColumnName.Length - 1, 1), view(i)(c.ColumnName))
                        Else
                            If view(i)(c.ColumnName) Then
                                xw.WriteElementString(c.ColumnName.Remove(c.ColumnName.Length - 1, 1), 1)
                            Else
                                xw.WriteElementString(c.ColumnName.Remove(c.ColumnName.Length - 1, 1), 0)
                            End If
                        End If
                        xw.WriteString(vbLf)
                    End If
                Next
                If indent = False Then
                    xw.WriteString(vbTab)
                    xw.WriteString(vbTab)
                End If
                xw.WriteEndElement()
                xw.WriteString(vbLf)
                'End STAND_MODIFIERS section
                'Start CROUCH_MODIFIERS section
                xw.WriteString(vbTab)
                xw.WriteString(vbTab)
                xw.WriteStartElement("CROUCH_MODIFIERS")
                indent = True
                For x = x To table.Rows(i).ItemArray.Length - 1
                    Dim c As DataColumn = table.Columns(x)
                    If c.ColumnName.EndsWith("3") Then Exit For
                    If Not trim OrElse (i = 0 OrElse c Is table.PrimaryKey(0) OrElse ((c.DataType.Equals(GetType(Boolean)) OrElse c.DataType.Equals(GetType(Decimal)) OrElse c.DataType.Equals(GetType(Integer))) AndAlso view(i)(c.ColumnName) <> -101) _
                       OrElse (c.DataType.Equals(GetType(String)) AndAlso view(i)(c.ColumnName) <> "")) Then
                        If indent = True Then
                            xw.WriteString(vbLf)
                            indent = False
                        End If
                        xw.WriteString(vbTab)
                        xw.WriteString(vbTab)
                        xw.WriteString(vbTab)
                        If Not c.DataType.Equals(GetType(Boolean)) Then
                            xw.WriteElementString(c.ColumnName.Remove(c.ColumnName.Length - 1, 1), view(i)(c.ColumnName))
                        Else
                            If view(i)(c.ColumnName) Then
                                xw.WriteElementString(c.ColumnName.Remove(c.ColumnName.Length - 1, 1), 1)
                            Else
                                xw.WriteElementString(c.ColumnName.Remove(c.ColumnName.Length - 1, 1), 0)
                            End If
                        End If
                        xw.WriteString(vbLf)
                    End If
                Next
                If indent = False Then
                    xw.WriteString(vbTab)
                    xw.WriteString(vbTab)
                End If
                xw.WriteEndElement()
                xw.WriteString(vbLf)
                'End CROUCH_MODIFIERS section
                'Start PRONE_MODIFIERS section
                xw.WriteString(vbTab)
                xw.WriteString(vbTab)
                xw.WriteStartElement("PRONE_MODIFIERS")
                indent = True
                For x = x To table.Rows(i).ItemArray.Length - 1
                    Dim c As DataColumn = table.Columns(x)
                    If Not trim OrElse (i = 0 OrElse c Is table.PrimaryKey(0) OrElse ((c.DataType.Equals(GetType(Boolean)) OrElse c.DataType.Equals(GetType(Decimal)) OrElse c.DataType.Equals(GetType(Integer))) AndAlso view(i)(c.ColumnName) <> -101) _
                       OrElse (c.DataType.Equals(GetType(String)) AndAlso view(i)(c.ColumnName) <> "")) Then
                        If indent = True Then
                            xw.WriteString(vbLf)
                            indent = False
                        End If
                        xw.WriteString(vbTab)
                        xw.WriteString(vbTab)
                        xw.WriteString(vbTab)
                        If Not c.DataType.Equals(GetType(Boolean)) Then
                            xw.WriteElementString(c.ColumnName.Remove(c.ColumnName.Length - 1, 1), view(i)(c.ColumnName))
                        Else
                            If view(i)(c.ColumnName) Then
                                xw.WriteElementString(c.ColumnName.Remove(c.ColumnName.Length - 1, 1), 1)
                            Else
                                xw.WriteElementString(c.ColumnName.Remove(c.ColumnName.Length - 1, 1), 0)
                            End If
                        End If
                        xw.WriteString(vbLf)
                    End If
                Next
                If indent = False Then
                    xw.WriteString(vbTab)
                    xw.WriteString(vbTab)
                End If
                xw.WriteEndElement()
                xw.WriteString(vbLf)
                'End PRONE_MODIFIERS section

                xw.WriteString(vbTab)
                xw.WriteEndElement()
                xw.WriteString(vbLf)
            End If
        Next
        xw.WriteEndElement()
        xw.WriteEndDocument()
        xw.Close()

    End Sub

    Protected Sub WriteXml_MercStartingGear(ByVal table As DataTable, ByVal fileName As String)
        'the stupid table.WriteXml method doesn't let you sort the data first
        Dim view As New DataView(table, "", table.Columns(0).ColumnName, DataViewRowState.CurrentRows)
        'Dim trim As Boolean = GetBooleanProperty(table, TableProperty.Trim)
        Dim trim As Boolean = False
        Dim sourceDSName = GetStringProperty(table, TableProperty.DataSetName)
        If sourceDSName Is Nothing Then
            If table.DataSet IsNot Nothing Then
                sourceDSName = table.DataSet.DataSetName
            Else
                sourceDSName = SchemaName
            End If
        End If

        Dim xw As New Xml.XmlTextWriter(fileName, Text.Encoding.UTF8)
        xw.WriteStartDocument()
        xw.WriteWhitespace(vbLf)
        xw.WriteStartElement(sourceDSName)
        xw.WriteWhitespace(vbLf)

        For i As Long = 0 To view.Count - 1
            If Not table.Rows(i).RowState = DataRowState.Deleted Then
                xw.WriteString(vbTab)
                xw.WriteStartElement(table.TableName)
                xw.WriteString(vbLf)

                'Dim dcIndex As Integer = -1
                Dim isGearKit As Boolean = False
                Dim gkIndex As String = ""
                trim = False

                For Each c As DataColumn In table.Columns

                    ' Close the <GEARKIT> element.  We want to do this here because we want to check for closing the <GEARKIT> tag after we've cycled columns
                    If Not gkIndex = c.ColumnName.Substring(c.ColumnName.Length - 1) And isGearKit = True Then
                        isGearKit = False
                        trim = True
                        xw.WriteString(vbTab)
                        xw.WriteString(vbTab)
                        xw.WriteEndElement()
                        xw.WriteString(vbLf)
                    End If

                    If Not trim OrElse (c Is table.PrimaryKey(0) OrElse ((c.DataType.Equals(GetType(Boolean)) OrElse c.DataType.Equals(GetType(Decimal)) OrElse c.DataType.Equals(GetType(Integer))) AndAlso view(i)(c.ColumnName) <> 0) _
                        OrElse (c.DataType.Equals(GetType(String)) AndAlso view(i)(c.ColumnName) <> "")) Then

                        xw.WriteString(vbTab)
                        xw.WriteString(vbTab)

                        ' Open the <GEARKIT> element
                        If (c.ColumnName.EndsWith("1") OrElse c.ColumnName.EndsWith("2") OrElse c.ColumnName.EndsWith("3") OrElse c.ColumnName.EndsWith("4") OrElse c.ColumnName.EndsWith("5")) _
                        And isGearKit = False Then
                            isGearKit = True
                            gkIndex = c.ColumnName.Substring(c.ColumnName.Length - 1)

                            xw.WriteStartElement("GEARKIT")
                            xw.WriteString(vbLf)
                            xw.WriteString(vbTab)
                            xw.WriteString(vbTab)
                        End If

                        If isGearKit = True Then
                            xw.WriteString(vbTab)
                        End If

                        If Not c.DataType.Equals(GetType(Boolean)) Then
                            If gkIndex = c.ColumnName.Substring(c.ColumnName.Length - 1) Then
                                xw.WriteElementString(c.ColumnName.Remove(c.ColumnName.Length - 1, 1), view(i)(c.ColumnName))
                            Else
                                xw.WriteElementString(c.ColumnName, view(i)(c.ColumnName))
                            End If
                        Else
                            If gkIndex = c.ColumnName.Substring(c.ColumnName.Length - 1) Then
                                If view(i)(c.ColumnName) Then
                                    xw.WriteElementString(c.ColumnName.Remove(c.ColumnName.Length - 1, 1), 1)
                                Else
                                    xw.WriteElementString(c.ColumnName.Remove(c.ColumnName.Length - 1, 1), 0)
                                End If
                            Else
                                If view(i)(c.ColumnName) Then
                                    xw.WriteElementString(c.ColumnName, 1)
                                Else
                                    xw.WriteElementString(c.ColumnName, 0)
                                End If
                            End If
                        End If

                        xw.WriteString(vbLf)
                    End If
                Next
                xw.WriteString(vbTab)
                If isGearKit = True Then
                    xw.WriteEndElement()
                    xw.WriteString(vbLf)
                    xw.WriteString(vbTab)
                End If
                xw.WriteEndElement()
                xw.WriteString(vbLf)
            End If
        Next
        xw.WriteEndElement()
        xw.Close()
        view.Dispose()

        table.AcceptChanges()
    End Sub

    Protected Sub WriteXml_Control(ByVal table As DataTable, ByVal fileName As String)
        Dim view As New DataView(table, "", table.Columns(0).ColumnName, DataViewRowState.CurrentRows)
        Dim xw As New XmlTextWriter(fileName, System.Text.Encoding.UTF8)
        Dim i As Integer
        Dim x As Integer
        Dim trim As Boolean = True
        Dim child1 As Boolean = False
        Dim child2 As Boolean = False

        xw.WriteStartDocument()
        xw.WriteWhitespace(vbLf)
        xw.WriteStartElement(table.ExtendedProperties("DataSetName").ToString)
        xw.WriteWhitespace(vbLf)
        For i = 0 To view.Count - 1
            child1 = False
            child2 = False
            If Not table.Rows(i).RowState = DataRowState.Deleted Then
                xw.WriteString(vbTab)
                xw.WriteStartElement(table.TableName)
                xw.WriteString(vbLf)
                For x = 0 To table.Rows(i).ItemArray.Length - 1
                    Dim c As DataColumn = table.Columns(x)
                    If c.ColumnName = "REORDERMINIMUM" Then
                        xw.WriteString(vbTab)
                        xw.WriteString(vbTab)
                        xw.WriteStartElement("REORDERDAYSDELAY")
                        xw.WriteWhitespace(vbLf)
                        child1 = True
                        child2 = False
                    ElseIf c.ColumnName = "INITIAL" Then
                        xw.WriteString(vbTab)
                        xw.WriteString(vbTab)
                        xw.WriteEndElement()
                        xw.WriteWhitespace(vbLf)
                        xw.WriteString(vbTab)
                        xw.WriteString(vbTab)
                        xw.WriteStartElement("CASH")
                        xw.WriteWhitespace(vbLf)
                        child1 = True
                        child2 = False
                    ElseIf c.ColumnName = "INCREMENT" Then
                        xw.WriteString(vbTab)
                        xw.WriteString(vbTab)
                        xw.WriteString(vbTab)
                        xw.WriteStartElement("DAILY")
                        xw.WriteWhitespace(vbLf)
                        child1 = False
                        child2 = True
                    ElseIf c.ColumnName = "COOLMINIMUM" Then
                        xw.WriteString(vbTab)
                        xw.WriteString(vbTab)
                        xw.WriteString(vbTab)
                        xw.WriteEndElement()
                        xw.WriteWhitespace(vbLf)
                        xw.WriteString(vbTab)
                        xw.WriteString(vbTab)
                        xw.WriteEndElement()
                        xw.WriteWhitespace(vbLf)
                        xw.WriteString(vbTab)
                        xw.WriteString(vbTab)
                        xw.WriteStartElement("COOLNESS")
                        xw.WriteWhitespace(vbLf)
                        child1 = True
                        child2 = False
                    ElseIf c.ColumnName = "ARMS_DEALER_HANDGUNCLASS" Then
                        xw.WriteString(vbTab)
                        xw.WriteString(vbTab)
                        xw.WriteEndElement()
                        xw.WriteWhitespace(vbLf)
                        xw.WriteString(vbTab)
                        xw.WriteString(vbTab)
                        xw.WriteStartElement("BASICDEALERFLAGS")
                        xw.WriteWhitespace(vbLf)
                        child1 = True
                        child2 = False
                    End If
                    If Not trim OrElse (c Is table.PrimaryKey(0) OrElse ((c.DataType.Equals(GetType(Boolean)) OrElse c.DataType.Equals(GetType(Decimal)) OrElse c.DataType.Equals(GetType(Integer))) AndAlso view(i)(c.ColumnName) <> 0) _
                        OrElse (c.DataType.Equals(GetType(String)) AndAlso view(i)(c.ColumnName) <> "")) Then
                        xw.WriteString(vbTab)
                        xw.WriteString(vbTab)
                        If child1 = True Then xw.WriteString(vbTab)
                        If child2 = True Then
                            xw.WriteString(vbTab)
                            xw.WriteString(vbTab)
                        End If
                        If Not c.DataType.Equals(GetType(Boolean)) Then
                            xw.WriteElementString(c.ColumnName, view(i)(c.ColumnName))
                        Else
                            If view(i)(c.ColumnName) Then
                                xw.WriteElementString(c.ColumnName, 1)
                            Else
                                xw.WriteElementString(c.ColumnName, 0)
                            End If
                        End If
                        xw.WriteString(vbLf)
                    End If
                Next

                xw.WriteString(vbTab)
                xw.WriteString(vbTab)
                xw.WriteEndElement()
                xw.WriteString(vbLf)

                xw.WriteString(vbTab)
                xw.WriteEndElement()
                xw.WriteString(vbLf)
            End If
        Next
        xw.WriteEndElement()
        xw.WriteEndDocument()
        xw.Close()

    End Sub

    Protected Sub WriteXml_Inventory(ByVal table As DataTable, ByVal fileName As String)
        Dim view As New DataView(table, "", table.Columns(0).ColumnName, DataViewRowState.CurrentRows)
        Dim xw As XmlTextWriter
        Dim control As Boolean = False
        Dim endTag As Byte() = System.Text.Encoding.UTF8.GetBytes("</" & table.ExtendedProperties("DataSetName").ToString & ">")
        If System.IO.File.Exists(fileName) Then
            Dim fs As System.IO.FileStream = System.IO.File.OpenWrite(fileName)
            fs.Seek(-endTag.Length, System.IO.SeekOrigin.End)
            xw = New XmlTextWriter(fs, System.Text.Encoding.UTF8)
            control = True
        Else
            xw = New XmlTextWriter(fileName, System.Text.Encoding.UTF8)
        End If
        Dim i As Integer
        Dim x As Integer
        Dim trim As Boolean = True

        If control = False Then
            xw.WriteStartDocument()
            xw.WriteWhitespace(vbLf)
            xw.WriteStartElement(table.ExtendedProperties("DataSetName").ToString)
            xw.WriteWhitespace(vbLf)
        End If
        For i = 0 To view.Count - 1
            If Not table.Rows(i).RowState = DataRowState.Deleted Then
                xw.WriteString(vbTab)
                xw.WriteStartElement(table.TableName)
                xw.WriteString(vbLf)
                For x = 0 To table.Rows(i).ItemArray.Length - 1
                    Dim c As DataColumn = table.Columns(x)
                    If Not trim OrElse (i = 0 OrElse c Is table.PrimaryKey(0) OrElse ((c.DataType.Equals(GetType(Boolean)) OrElse c.DataType.Equals(GetType(Decimal)) OrElse c.DataType.Equals(GetType(Integer))) AndAlso view(i)(c.ColumnName) <> 0) _
                        OrElse (c.DataType.Equals(GetType(String)) AndAlso view(i)(c.ColumnName) <> "")) Then
                        xw.WriteString(vbTab)
                        xw.WriteString(vbTab)
                        If Not c.DataType.Equals(GetType(Boolean)) Then
                            xw.WriteElementString(c.ColumnName, view(i)(c.ColumnName))
                        Else
                            If view(i)(c.ColumnName) Then
                                xw.WriteElementString(c.ColumnName, 1)
                            Else
                                xw.WriteElementString(c.ColumnName, 0)
                            End If
                        End If
                        xw.WriteString(vbLf)
                    End If
                Next

                xw.WriteString(vbTab)
                xw.WriteEndElement()
                xw.WriteString(vbLf)
            End If
        Next
        If control = False Then
            xw.WriteEndElement()
            xw.WriteEndDocument()
        Else
            xw.Flush()
            xw.WriteRaw("</" & table.ExtendedProperties("DataSetName").ToString & ">")
        End If
        xw.Close()

    End Sub

    'this only works when the pk is an integer, which it should be 99% of the time in our xml files anyway
    'it also just works on single pk tables for now
    Public Overridable Function GetNextPrimaryKeyValue() As Integer
        Dim pk As String = tbl.PrimaryKey(0).ColumnName
        If tbl.Rows.Count > 0 Then
            Return tbl.Compute("MAX(" & pk & ")", Nothing) + 1
        Else
            Return 0
        End If
    End Function

    'only works w/single key values
    Public Overridable Function NewRow() As DataRow
        Dim row As DataRow = tbl.NewRow
        row(tbl.PrimaryKey(0)) = GetNextPrimaryKeyValue()
        tbl.Rows.Add(row)
        Return row
    End Function

    Public Overridable Sub DeleteRow(ByVal key As Integer)
        Dim row As DataRow = tbl.Rows.Find(key)
        If row IsNot Nothing Then row.Delete()
    End Sub

    'only works when there's a single key table at the top of the relation
    Public Overridable Function DuplicateRow(ByVal key As Integer) As DataRow
        Dim row As DataRow = tbl.Rows.Find(key)
        If row IsNot Nothing Then
            Return DuplicateRows(New DataRow() {row}, Nothing, key)
        Else
            Return Nothing
        End If
    End Function

    Protected Function DuplicateRows(ByVal rows() As DataRow, ByVal parentRow As DataRow, ByVal baseKey As Integer) As DataRow
        Dim dupeRow As DataRow = Nothing
        Dim canAddRow As Boolean
        For Each row As DataRow In rows
            canAddRow = False
            dupeRow = row.Table.NewRow
            For Each c As DataColumn In row.Table.Columns
                If Not c.ReadOnly Then dupeRow(c) = row(c)
            Next

            If dupeRow.Table.PrimaryKey.Length = 1 Then
                dupeRow(dupeRow.Table.PrimaryKey(0)) = GetTableHandler(row.Table).GetNextPrimaryKeyValue()
                If parentRow IsNot Nothing Then dupeRow.SetParentRow(parentRow)
                canAddRow = True
            ElseIf parentRow IsNot Nothing Then
                For i As Integer = 0 To dupeRow.Table.PrimaryKey.Length - 1
                    If dupeRow(dupeRow.Table.PrimaryKey(i)) = baseKey Then
                        dupeRow(dupeRow.Table.PrimaryKey(i)) = parentRow(parentRow.Table.PrimaryKey(0))
                        canAddRow = True
                    End If
                Next
            End If
            If canAddRow Then
                Try
                    row.Table.Rows.Add(dupeRow)
                Catch ex As Exception

                End Try
            End If

            For Each dr As DataRelation In row.Table.ChildRelations
                DuplicateRows(row.GetChildRows(dr), dupeRow, baseKey)
            Next
        Next
        Return dupeRow 'return last row copied
    End Function

    Protected Overrides Sub Finalize()
        MyBase.Finalize()
    End Sub
End Class
