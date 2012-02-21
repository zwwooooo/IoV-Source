Public Class IniFile
    Protected Shared dataDir As String
    Protected Shared apMax As Integer

    Protected Shared languageSpecificRussianGameDirPath As String = ""
    Protected Shared languageSpecificPolishGameDirPath As String = ""
    Protected Shared languageSpecificGermanGameDirPath As String = ""
    Protected Shared languageSpecificItalianGameDirPath As String = ""
    Protected Shared languageSpecificFrenchGameDirPath As String = ""
    Protected Shared languageSpecificDutchGameDirPath As String = ""
    Protected Shared languageSpecificChineseGameDirPath As String = ""
    Protected Shared languageSpecificTaiwaneseGameDirPath As String = ""

    Public Shared ReadOnly Property DataDirectory() As String
        Get
            Return dataDir
        End Get
    End Property

    Public Shared ReadOnly Property APMaximum() As Integer
        Get
            Return apMax
        End Get
    End Property

    Public Shared ReadOnly Property LanguageSpecific_Russian_GameDirPath() As String
        Get
            Return languageSpecificRussianGameDirPath
        End Get
    End Property

    Public Shared ReadOnly Property LanguageSpecific_Polish_GameDirPath() As String
        Get
            Return languageSpecificPolishGameDirPath
        End Get
    End Property

    Public Shared ReadOnly Property LanguageSpecific_German_GameDirPath() As String
        Get
            Return languageSpecificGermanGameDirPath
        End Get
    End Property

    Public Shared ReadOnly Property LanguageSpecific_Italian_GameDirPath() As String
        Get
            Return languageSpecificItalianGameDirPath
        End Get
    End Property

    Public Shared ReadOnly Property LanguageSpecific_French_GameDirPath() As String
        Get
            Return languageSpecificFrenchGameDirPath
        End Get
    End Property

    Public Shared ReadOnly Property LanguageSpecific_Chinese_GameDirPath() As String
        Get
            Return languageSpecificChineseGameDirPath
        End Get
    End Property

    Public Shared ReadOnly Property LanguageSpecific_Taiwanese_GameDirPath() As String
        Get
            Return languageSpecificTaiwaneseGameDirPath
        End Get
    End Property

    Public Shared ReadOnly Property LanguageSpecific_Dutch_GameDirPath() As String
        Get
            Return languageSpecificDutchGameDirPath
        End Get
    End Property

    Public Shared Sub ReadFile(ByVal fileName As String)
        Dim xr As New Xml.XmlTextReader(fileName)
        Dim curNode As String = ""
        Dim curValue As String = ""
        While xr.Read
            If xr.NodeType = Xml.XmlNodeType.Element Then
                curNode = xr.Name
            ElseIf xr.NodeType = Xml.XmlNodeType.Text Then
                curValue = xr.Value
                Select Case curNode
                    Case "Data_Directory"
                        dataDir = curValue
                        If Not dataDir.EndsWith("\") Then dataDir &= "\"
                    Case "AP_Maximum"
                        apMax = curValue

                    Case "GameDir_Russian_Path"
                        languageSpecificRussianGameDirPath = curValue
                    Case "GameDir_German_Path"
                        languageSpecificGermanGameDirPath = curValue
                    Case "GameDir_Polish_Path"
                        languageSpecificPolishGameDirPath = curValue
                    Case "GameDir_French_Path"
                        languageSpecificFrenchGameDirPath = curValue
                    Case "GameDir_Italian_Path"
                        languageSpecificItalianGameDirPath = curValue
                    Case "GameDir_Chinese_Path"
                        languageSpecificChineseGameDirPath = curValue
                    Case "GameDir_Dutch_Path"
                        languageSpecificDutchGameDirPath = curValue
                    Case "GameDir_Taiwanese_Path"
                        languageSpecificTaiwaneseGameDirPath = curValue

                End Select
            End If
        End While
        xr.Close()
    End Sub

End Class
