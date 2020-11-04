Imports System
Imports System.IO.Ports
Imports System.Runtime.InteropServices

Public Class Form1
    Dim comPORT As String          '宣告變數com端端口'
    Dim data As String              '燈的顏色'
    Dim receivedData As String  '宣告變數要接收com 上的數據'
    Dim commandCount As Integer = 0
    Private Sub Form1_Load(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles MyBase.Load
        Timer1.Enabled = False
        Timer2.Enabled = True
        comPORT = ""  '將電腦接到的所有com端組合併至下拉選單給使用者選擇'                                    
        For Each sp As String In My.Computer.Ports.SerialPortNames
            ComboBox1.Items.Add(sp)
        Next
    End Sub
    Private Sub Button1_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button1.Click
        If (Button1.Text = "Connect") Then                           '如果按鈕文字是"Connect"執行...
            If (comPORT <> "") Then                                  '如果comPORT不等於空(選擇了COM點)執行...
                SerialPort1.Close()                                  '初始化Serial設定
                SerialPort1.PortName = comPORT                       '將comPORT端口放入串行端口名稱
                SerialPort1.BaudRate = 38400                         '將串行端口鮑率設為38400'
                SerialPort1.DataBits = 8                             '資料位元設為8位元'
                SerialPort1.Parity = Parity.None                     '設為無同位檢查'
                SerialPort1.StopBits = StopBits.One                  '停止位元設為1
                SerialPort1.Handshake = Handshake.None
                SerialPort1.Encoding = System.Text.Encoding.Default '****取得作業系統編碼方式
                SerialPort1.ReadTimeout = 10000
                '打開SerialPort1
                SerialPort1.Open()
                RichTextBox1.Text = ""
                Button1.Text = "Dis-connect"
                ComboBox1.Enabled = False
                Timer1.Enabled = True
                Label3.Text = "Timer: ON"
                OvalShape9.BackColor = Color.Green
                LED()
            Else
                MsgBox("Select a COM port first")                   '顯示訊息要連結com點
            End If
        Else
            Timer1.Enabled = False
            SerialPort1.Close()                                     '關閉Serial
            Button1.Text = "Connect"                                '改變文字為Connect
            ComboBox1.Enabled = True                                '重新選擇comPORT
            OvalShape9.BackColor = Color.Red
            Label3.Text = "Timer: OFF"
            ComboBox1.Text = String.Empty                           'ComboBox顯示為空
        End If
    End Sub
    Private Sub ComboBox1_SelectedIndexChanged(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles ComboBox1.SelectedIndexChanged
        If (ComboBox1.SelectedItem <> "") Then  '當用戶選擇COM端口時，將值放到comPORT'
            comPORT = ComboBox1.SelectedItem
        End If
    End Sub
    Private Const WM_VSCROLL As Int32 = &H115
    Private Const SB_BOTTOM As Int32 = 7
    Dim textIndex As Integer = 0
    Private Sub Timer1_Tick(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Timer1.Tick
        Timer1.Enabled = False
        Label3.Text = "Timer: OFF"
        receivedData = ReceiveSerialData() '將串行的數據傳到receivedData裡'
        RichTextBox1.AppendText(receivedData)

        Timer1.Enabled = True
        Label3.Text = "Timer: ON"
        If (textIndex Mod 150) = 0 Then
            RichTextBox1.Text = ""
        End If
        textIndex = textIndex + 1
    End Sub
    Function ReceiveSerialData() As String
        Dim Incoming As String
        Try
            Incoming = SerialPort1.ReadExisting()
            If Incoming Is Nothing Then
                Return "nothing" & vbCrLf
            Else
                Return Incoming
            End If
        Catch ex As Exception
            Return "Error: Serial Port  read timed out."
        End Try
    End Function
    Private Sub Button2_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button2.Click
        RichTextBox1.Text = ""      '將文本框的數據清除掉'
    End Sub
    Private Sub Button3_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button3.Click
        SerialPort1.WriteLine(TextBox1.Text)  '將資料寫入串行'
    End Sub
    Private Sub Button4_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button4.Click
        Console.Write(SerialPort1.ReadLine()) ' '
    End Sub
    Private Sub Button5_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button5.Click
        SerialPort1.Write("R")
    End Sub
    Private Sub Button6_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button6.Click
        SerialPort1.Write("O")
    End Sub
    Private Sub Button7_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button7.Click
        SerialPort1.Write("Y")
    End Sub
    Private Sub Button8_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button8.Click
        SerialPort1.Write("G")
    End Sub
    Private Sub Button9_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button9.Click
        SerialPort1.Write("B")
    End Sub
    Private Sub Button10_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button10.Click
        SerialPort1.Write("I")
    End Sub
    Private Sub Button11_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button11.Click
        SerialPort1.Write("P")
    End Sub
    Private Sub Button12_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button12.Click
        SerialPort1.Write("W")
    End Sub
    Private Sub Button13_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button13.Click
        SerialPort1.Write("9")
    End Sub
    Private Sub Button14_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button14.Click
        SerialPort1.Write("a")
    End Sub
    Private Sub Button15_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button15.Click
        SerialPort1.Write("b")
    End Sub
    Private Sub Button16_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button16.Click
        SerialPort1.Write("c")
    End Sub
    Private Sub Button18_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button18.Click
        SerialPort1.Write("d")
    End Sub
    Private Sub Button19_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button19.Click
        SerialPort1.Write("e")
    End Sub
    Private Sub Button20_Click(ByVal sender As Object, ByVal e As System.EventArgs) Handles Button20.Click
        SerialPort1.Write("f")
    End Sub
    Private Sub Button21_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button21.Click
        SerialPort1.Write("}")
    End Sub
    Private Sub Button22_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button22.Click
        SerialPort1.Write("{")
    End Sub
    Private Sub Button23_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button23.Click
        SerialPort1.Write("0")
    End Sub
    Private Sub Button24_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button24.Click
        Dim selectedItem As String = ComboBox3.Items(ComboBox3.SelectedIndex)
        SerialPort1.Write(selectedItem)
    End Sub
    Private Sub Button25_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button25.Click
        SerialPort1.Write("@")
    End Sub
    Private Sub Timer2_Tick(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Timer2.Tick
        Label7.Text = "Current Time:" & Format(Now(), "hh:mm:ss")
        Label5.Text = "Computer Name:" & Environ$("computername")
        Dim allDrives() As System.IO.DriveInfo = System.IO.DriveInfo.GetDrives()
        Dim d As System.IO.DriveInfo
        Dim strMsg As String = ""
        For Each d In allDrives
            If d.Name = "C:\" Then
                strMsg = strMsg & String.Format("DiskName: {0}", d.Name)
                If d.IsReady = True Then            '磁碟是否就緒
                    strMsg = strMsg & String.Format(vbCrLf & "AvailableFreeSpace:{0}Bytes", d.AvailableFreeSpace)
                End If
            End If
        Next
        Label8.Text = (strMsg)
    End Sub
    Private Sub LED()
        OvalShape1.BackColor = Color.Olive
        OvalShape2.BackColor = Color.Olive
        OvalShape3.BackColor = Color.Olive
        OvalShape4.BackColor = Color.Olive
        OvalShape5.BackColor = Color.Olive
        OvalShape6.BackColor = Color.Olive
        OvalShape7.BackColor = Color.Olive
        OvalShape8.BackColor = Color.Olive
    End Sub
    Private Sub Button27_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button27.Click
        SerialPort1.Write("A")
    End Sub
End Class



