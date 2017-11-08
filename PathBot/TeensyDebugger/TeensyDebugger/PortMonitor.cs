using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO.Ports;

namespace TeensyDebugger
{
    class PortsMonitor
    {
        Task monitor;
        bool quitRequest = false;
        int refresh = 1000;

        public event EventHandler TargetAvailable;

        string[] availablePorts;
        public string[] AvailablePorts
        {
            get
            {
                return availablePorts;
            }
            set
            {
                availablePorts = value;
                if (availablePorts.Contains(TargetPort) && TargetAvailable!=null)
                {
                    TargetAvailable.Invoke(this, null);
                }
            }
        }

        string targetPort;
        public string TargetPort
        {
            get
            {
                return targetPort;
            }
            set
            {
                targetPort = value;
                if(AvailablePorts.Contains(targetPort) && TargetAvailable != null)
                {
                    TargetAvailable.Invoke(this, null);
                }
            }
        }

        public void Stop()
        {
            quitRequest = true;
            if (monitor != null && !monitor.IsCompleted)
                monitor.Wait();
        }

        public PortsMonitor()
        {
            monitor = new Task(monitorRoutine);
            quitRequest = false;
            monitor.Start();
        }

        async void monitorRoutine()
        {
            while(!quitRequest)
            {
                AvailablePorts = SerialPort.GetPortNames();

                await Task.Delay(refresh);
            }
        }
    }
}
