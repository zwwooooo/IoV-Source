Public Module Localization
    Public Function IsLanguageSpecificFile(ByVal filename As String) As Boolean
        Dim langSpec = False

        Dim language = GetLanguageFromFile(filename)
        If (language <> "") Then
            langSpec = True
        End If

        Return langSpec

    End Function

    Public Function GetLanguageFromFile(ByVal filename As String) As String

        Dim language = ""   ' English -> Language neutral

        If filename.StartsWith("German.") Then
            language = "German"
        ElseIf filename.StartsWith("Russian.") Then
            language = "Russian"
        ElseIf filename.StartsWith("Polish.") Then
            language = "Polish"
        ElseIf filename.StartsWith("Italian.") Then
            language = "Italian"
        ElseIf filename.StartsWith("French.") Then
            language = "French"
        ElseIf filename.StartsWith("Chinese.") Then
            language = "Chinese"
        ElseIf filename.StartsWith("Taiwanese.") Then
            language = "Taiwanese"
        ElseIf filename.StartsWith("Dutch.") Then
            language = "Dutch"
        End If

        Return language

    End Function

End Module
