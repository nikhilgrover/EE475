using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO.Ports;
using System.Threading.Tasks;

namespace TeensyDebugger
{
    public delegate void SerialPortEventHandler(object sender, SerialPortEventArgs e);

    public class SerialPortEventArgs : EventArgs
    {
        public SerialPort Port{get;protected set;}
        public bool Handled{get;set;}
        public int AvailableBytes { get; protected set; }
        
        public SerialPortEventArgs(SerialPort port, int availableBytes)
        {
            this.Port = port;
            Handled = false;
            this.AvailableBytes = availableBytes;
        }
    }

    public class UARTMonitor
    {
        Task monitor;
        bool quitRequested = false;
        SerialPort port;
        public SerialPort Port
        {
            get
            {
                return port;
            }
            set
            {
                port = value;
                unHandledBytes = 0;
            }
        }
        public int Refresh { get; set; }

        public event SerialPortEventHandler Activity;

        public UARTMonitor()
        {
            monitor = new Task(monitorRoutine);
            Refresh = 10;
            EnableMonitor();
            monitor.Start();
        }

        public void Stop()
        {
            quitRequested = true;
            monitor.Wait();
        }

        public void DisableMonitor()
        {
            disabledMonitor = false;
            disableMonitor = true;      
            System.Threading.SpinWait.SpinUntil(() => disabledMonitor);
        }

        public void EnableMonitor()
        {
            disableMonitor = false;
        }

        int unHandledBytes = 0;
        bool disableMonitor = false;
        bool disabledMonitor = false;
        
        async void monitorRoutine()
        {
            while (!quitRequested)
            {
                if(disableMonitor)
                {
                    disabledMonitor = true;
                }
                else if (Port != null && Port.IsOpen)
                {
                    int temp = Port.BytesToRead;
                    if (temp>unHandledBytes && Activity != null)
                    {
                        SerialPortEventArgs e = new SerialPortEventArgs(port, temp);
                        Activity.Invoke(this, e);
                        if (!e.Handled)
                            unHandledBytes += temp;
                    }

                }
                await Task.Delay(Refresh);
            }
        }

        public int ReadBytes(byte[] buffer, int offset, int count)
        {
            if (Port == null || !Port.IsOpen)
                return -1;
            int bytesRead = Port.BaseStream.Read(buffer, offset, count);
            return bytesRead;
        }
    }
}
