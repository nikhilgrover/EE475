using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Diagnostics;
using System.IO;
using System.IO.Ports;

namespace TeensyDebugger
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        DisassemblyManager manager = new DisassemblyManager();
        PortsMonitor monitor = new PortsMonitor();

        SerialPort serialPort = null;
        bool downloading = false;
        UARTMonitor inputTask = null;
        FastColoredTextBoxNS.TextStyle errorStyle;

        void debug()
        {
            if (serialPort == null || !serialPort.IsOpen)
                return;

            int serialTimeout = 10;//ms


            inputTask.DisableMonitor();


            //flush pre-existing data
            serialPort.DiscardInBuffer();
            //trigger ISR through data transmission
            serialPort.Write(new char[] { '$' }, 0, 1);//0=start index, 1=length

            SpinWait.SpinUntil(() => serialPort.BytesToRead > 1, serialTimeout);
            if (serialPort.BytesToRead < 1)
                return;

            //max receivable bytes
            int memLimit = 4096;
            //current index in buffer array
            int memIndex = 0;
            //number of bytes to read in an iteration
            int memIncrement = 0;
            //storage for received bytes
            byte[] buffer = new byte[memLimit];


            



            //listen to serial port until 10ms have elapsed without any 
            //data transmission or until 4KB has been received
            while (serialPort.BytesToRead > 0 && memIndex < memLimit)
            {
                //because BytesToRead can be increasing line-by-line, capture it once
                //and use as reference until next iteration
                memIncrement = serialPort.BytesToRead;
                memIndex += serialPort.Read(buffer, memIndex, memIncrement);
                //wait until more data or timeout
                if (!SpinWait.SpinUntil(() => serialPort.BytesToRead > 0, serialTimeout))
                    break;//timeout occured; jump out of loop
            }
            inputTask.EnableMonitor();
            //explicitly stop serial connection
            //serialPort.Close();

            //convert ASCII encoded byte[] to a string
            string str = System.Text.Encoding.ASCII.GetString(buffer, 0, memIndex);

            //txtbxOutput.AppendText(str);
            //create a new stack dump object for converting the stack data to a data object
            StackDump d = new StackDump(str, manager);
            //the magic happens here
            Stack<AsmLabel> stack = d.Unwind();

            this.stackFlowLayout1.UpdateStack(stack);

            //copy stack to an iterable array
            AsmLabel[] stackarray = new AsmLabel[stack.Count];
            stack.CopyTo(stackarray, 0);

            //print data to GUI
            foreach (AsmLabel lbl in stackarray)
                txtbxOutput.AppendText(lbl.ToString() + "\r\n");

            txtbxOutput.AppendText("\r\n");
            this.txtbxOutput.Navigate(txtbxOutput.Lines.Count - 1);
        }

        private void btnBreak_Click(object sender, EventArgs e)
        {
            debug();    
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            //read disassembly file
            manager.Filepath = "C:\\Teensy\\Projects\\HelloWorld\\Main_Asm.txt";
            manager.Load();
            manager.ParseLabels();
            //cmbbxPort.Items.Clear();
            //cmbbxPort.Items.AddRange(monitor.AvailablePorts);

            monitor.TargetAvailable += monitor_TargetAvailable;
            monitor.TargetPort = "COM3";
            compile.ProcessOutputHandler += compile_ProcessOutputHandler;

            compile.OnComplete += compile_OnComplete;
            reboot.OnComplete += compile_OnComplete;

            
            serialPort = new SerialPort(monitor.TargetPort, 9600, Parity.None, 8, StopBits.One);
            serialPort.Open();

            inputTask = new UARTMonitor();
            inputTask.Port = serialPort;
            inputTask.Activity += inputTask_Activity;

            errorStyle = new FastColoredTextBoxNS.TextStyle(Brushes.Red, Brushes.White, FontStyle.Regular);


            this.stackFlowLayout1.Server = new PacketStateMachine("NPtest");
            if(!this.stackFlowLayout1.Server.Start())
            {
                MessageBox.Show("Server could not be started");
                return;
            }

        }

        bool debugging = false;

        void inputTask_Activity(object sender, SerialPortEventArgs e)
        {
            byte[] buffer = new byte[e.AvailableBytes];

            inputTask.ReadBytes(buffer, 0, buffer.Length);

            string str = System.Text.Encoding.ASCII.GetString(buffer, 0, buffer.Length);

            this.Invoke((Action)(() =>
                { 
                    txtbxOutput.AppendText(str);
                    txtbxOutput.Navigate(txtbxOutput.Lines.Count - 1);
                }));

            e.Handled = true;
        }

        void compile_OnComplete()
        {
            downloading = false;
            manager.Load();
            manager.ParseLabels();
            if (System.IO.Ports.SerialPort.GetPortNames().Contains(monitor.TargetPort))
                serialPort.Open();
        }

        void monitor_TargetAvailable(object sender, EventArgs e)
        {
            if (!downloading && serialPort!= null && !serialPort.IsOpen)
            {
                serialPort.Open();
            }
        }

        void compile_ProcessOutputHandler(object sender, ProcessEventArgs e)
        {
            this.Invoke((Action)(() =>
            {
                txtbxOutput.AppendText(e.Output + "\r\n");

                if(!string.IsNullOrWhiteSpace(e.Error) && e.Error.Contains(": error:"))
                {
                    txtbxOutput.AppendText(e.Error + "\r\n", errorStyle);
                }
                else
                {
                    txtbxOutput.AppendText(e.Error + "\r\n");
                }

                
                txtbxOutput.Navigate(txtbxOutput.Lines.Count - 1);
            }));
        }

        CmdLineExec compile = new CmdLineExec(new string[] { "cd C:\\Teensy\\Projects\\HelloWorld", "make", "C:\\Teensy\\Compiler\\arm\\bin\\arm-none-eabi-objdump -d main.elf > Main_Asm.txt", "exit"});
        CmdLineExec reboot = new CmdLineExec(new string[] { "C:\\Teensy\\Compiler\\teensy_reboot", "exit" });
        private void btnCompile_Click(object sender, EventArgs e)
        {
            downloading = true;
            if (serialPort != null && serialPort.IsOpen)
                serialPort.Close();
            if (!compile.Run())
            {
                DialogResult decision = MessageBox.Show("A compilation is already running. Do you want to restart?", "", MessageBoxButtons.YesNo);
                if (decision == DialogResult.Yes)
                {
                    compile.Cancel();
                    compile.Run();
                }
            }
        }

        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
            compile.Cancel();
            inputTask.Stop();
        }

        private void cmbbxPort_SelectedIndexChanged(object sender, EventArgs e)
        {

        }

        private void btnClear_Click(object sender, EventArgs e)
        {
            txtbxOutput.Clear();
        }

        private void btnRestart_Click(object sender, EventArgs e)
        {
            downloading = true;
            if (serialPort != null && serialPort.IsOpen)
                serialPort.Close();

            if (Process.GetProcessesByName("Teensy").Length < 1)
                Process.Start("C:\\Teensy\\Compiler\\teensy.exe");

            reboot.Run();
        }

        private void txtbxCmd_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Enter)
            {
                txtbxCmd.Text = "";

                txtbxCmd.Clear();

                //txtbxCmd.Lines = null;// new string[] { };
                txtbxCmd.ResetText();
                e.SuppressKeyPress = true;
            }
            
        }


        private void btnStop_Click(object sender, EventArgs e)
        {
            if (btnStop.Tag == null)
            {
                downloading = true;
                if (serialPort != null && serialPort.IsOpen)
                    serialPort.Close();
                btnStop.Tag = 1;
            }
            else
            {
                downloading = false;
                if (System.IO.Ports.SerialPort.GetPortNames().Contains(monitor.TargetPort) && !serialPort.IsOpen)
                    serialPort.Open();
                btnStop.Tag = null;
            }
        }

        private void linkLabel1_LinkClicked(object sender, LinkLabelLinkClickedEventArgs e)
        {

        }
    }
}
