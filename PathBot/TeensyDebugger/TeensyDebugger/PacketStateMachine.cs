using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Diagnostics;

namespace TeensyDebugger
{
    public class PacketStateMachine
    {
      

        public void ProcessData(byte[] packet)
        {

        }

        public void Send(byte[] data)
        {
            byte[] packet = new byte[data.Length + 4];
            byte[] length = BitConverter.GetBytes((UInt32)data.Length);
            for (int i = 0; i < length.Length; i++)
                packet[i] = length[i];
            for (int i = 0; i < data.Length; i++)
                packet[length.Length + i] = data[i];
            pipe.Outbox.Enqueue(packet);
        }

        PipeManager pipe;
        public bool Start()
        {
            return pipe.Connect();
        }
        public PacketStateMachine(string ServerAddr)
        {
            pipe = new PipeManager(ServerAddr);
        }

    }
}
