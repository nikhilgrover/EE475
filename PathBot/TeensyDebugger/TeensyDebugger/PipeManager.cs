using System;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Collections.Concurrent;
using System.IO;
using System.IO.Pipes;

namespace TeensyDebugger
{
    class PipeManager
    {
        //virtual threads used for reading and writing to byte streams, since the pipe read/write operations are blocking
        Task ReaderTask = null;
        Task WriterTask = null;

        //status bools for connection
        bool readConnected = false;
        bool writeConnected = false;

        //when set true, all tasks exit
        public bool quitReq = false;

        //number of ms to wait before polling streams
        int pollSleep = 10;

        //server of the pipe. Other processes can access this pipe with the PipeAddr
        NamedPipeServerStream pipeServer = null;

        //stream objects. These are controlled through the pipeServer
        BinaryReader readStream = null;
        BinaryWriter writeStream = null;
        //address of the pipe
        string PipeAddr { get; set; }

        //needs implementation
        public StringBuilder Input { get; protected set; }
        public StringBuilder Output { get; protected set; }

        //rolling buffer that stores most recent data (4KB)
        //0th byte is not the most recent byte. The most recent byte
        //is at the index specified by OutputIndex;
        //Data is accessed by reading from BufferReadPos to OutputIndex
        byte[] OutputBuffer = new byte[4096];

        //the last read byte from the readStream.
        int OutputIndex;
        //is set true when readStream has more bytes available than
        //the number of element from OutputIndex to the end of the buffer.
        //when wrapsAround is true, the data wraps from the end of the array
        //back to the beginning
        bool wrapsAround = false;
        //the next byte to read from the buffer.
        int BufferReadPos = 0;
        public ConcurrentQueue<byte[]> Outbox { get; set; }


        public PipeManager(string PipeAddr)
        {
            this.PipeAddr = PipeAddr;
            Outbox = new ConcurrentQueue<byte[]>();
        }

        //routine for connection handling. Only used by task instantiated in Connect()->InitStream()
        async void HandleConnection()
        {
            Console.WriteLine("Waiting for connection...");

            //FIX THIS:
            //pipe server waits for connection before checking quitReq bool.
            //polling pipeServer.IsConnected does not work
            pipeServer.WaitForConnection();
            //while(!pipeServer.IsConnected && !quitReq)
            //    await Task.Delay(pollSleep);

            //if a quit is requested, get out now
            if (quitReq)
                return;

            Console.WriteLine("Connected.");
            //connnect streams and update connection flags.
            //the read and write tasks have already been started, but are waiting for connection flags
            readStream = new BinaryReader(pipeServer);
            writeStream = new BinaryWriter(pipeServer);
            readConnected = true;
            writeConnected = true;
        }

        //internal method for starting the pipe stream
        bool InitStream()
        {
            //reset flags
            readConnected = false;
            writeConnected = false;

            //dispose of old connections, if they exist
            if (pipeServer != null)
                pipeServer.Close();
            if (readStream != null)
                readStream.Close();
            if (writeStream != null)
                writeStream.Close();

            //starts a new server
            pipeServer = new NamedPipeServerStream(PipeAddr);

            //starts the connection handling task
            Task.Factory.StartNew(HandleConnection);

            //consider changing this function to void since it never returns false
            return true;
        }

        /// <summary>
        /// call this to start the pipe server
        /// </summary>
        /// <returns>true on success, false on failure</returns>
        public bool Connect()
        {
            //start server pipe stream and the connection handling task
            if (!InitStream())
                return false;
            quitReq = false;

            //start reader and writer tasks
            ReaderTask = new Task(readRoutine);
            WriterTask = new Task(writeRoutine);
            ReaderTask.Start();
            WriterTask.Start();
            return true;
        }

        /// <summary>
        /// Blocking function that orders and waits for all COMMs activity to cease
        /// </summary>
        public void Disconnect()
        {
            //set quit request flag true
            quitReq = true;
            //wait for all tasks to exit 
            Task.WaitAll(ReaderTask, WriterTask);
            //FIX:
            //add connection handling task to Task.WaitAll call
        }

        //routine for read task
        async void readRoutine()
        {
            //always exit when quitReq is true

            byte[] lengthBytes = new byte[4];

            while (!quitReq)
            {
                if (!readConnected)
                {
                    await Task.Delay(pollSleep);
                    continue;
                }

                for (int i = 0; i < 4; i++)
                    lengthBytes[i] = readStream.ReadByte();

                int length = (int)BitConverter.ToUInt32(lengthBytes, 0);



                int receivedLength = 0;
                if (length < 1)
                {
                    if (!writeConnected || Outbox.IsEmpty)
                    {
                        await Task.Delay(pollSleep);
                        continue;
                    }

                    byte[] data;
                    while (!Outbox.TryDequeue(out data))
                    { }
                    writeStream.Write(data);
                    continue;
                }

                //if array needs folding
                if (OutputIndex + length > OutputBuffer.Length)
                {
                    int lengthToEnd = OutputBuffer.Length - OutputIndex;
                    int remainder = length - lengthToEnd;

                    while (receivedLength < lengthToEnd)
                        receivedLength += readStream.Read(OutputBuffer, OutputIndex, length - remainder - receivedLength);
                    OutputIndex = 0;
                    wrapsAround = true;
                    while (receivedLength < lengthToEnd + remainder)
                        receivedLength += readStream.Read(OutputBuffer, OutputIndex, length - receivedLength);
                    OutputIndex = remainder;
                }
                //else we're fine with one read
                else
                {
                    while (receivedLength < length)
                        receivedLength += readStream.Read(OutputBuffer, OutputIndex, length - receivedLength);

                    OutputIndex += length;
                }


                int msgLength = 0;
                byte[] tempBuffer = null;
                if (!wrapsAround)
                {
                    msgLength = OutputIndex - BufferReadPos;
                    tempBuffer = new byte[msgLength];
                    Array.Copy(OutputBuffer, BufferReadPos, tempBuffer, 0, msgLength);
                    BufferReadPos = OutputIndex;
                }
                else
                {
                    msgLength = OutputBuffer.Length - BufferReadPos;
                    int msgLength2 = OutputIndex;

                    tempBuffer = new byte[msgLength + msgLength2];
                    Array.Copy(OutputBuffer, BufferReadPos, tempBuffer, 0, msgLength);
                    Array.Copy(OutputBuffer, 0, tempBuffer, msgLength, msgLength2);
                    wrapsAround = false;
                    BufferReadPos = OutputIndex;
                }

                //Req temp = new Req(ReqType.Blank);
                //temp.Deserialize(tempBuffer);

                //switch (temp.PacketType)
                //{
                //    case ReqType.Blank:
                //        //error!
                //        break;
                //    case ReqType.Status:

                //        break;
                //    default:

                //        break;
                //}


            }

            //FIX THIS: add stream disconnection here? In Disconnect method?
        }

        async void writeRoutine()
        {
            while (!quitReq)
            {
                //if (!writeConnected || Outbox.IsEmpty)
                //{
                //    await Task.Delay(pollSleep);
                //    continue;
                //}
                await Task.Delay(pollSleep);
                //byte[] data;
                //while (!Outbox.TryDequeue(out data))
                //{ }
                //writeStream.Write(data);
            }
        }
    }
}
