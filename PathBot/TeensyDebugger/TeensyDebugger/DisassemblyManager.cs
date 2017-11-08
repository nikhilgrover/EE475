using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;

namespace TeensyDebugger
{

    public class AsmLabel
    {
        public string Name { get; set; }
        public UInt32 StartAddr { get; set; }
        public long CharPosition { get; set; }
        public long LineNumber { get; set; }
        public string[] StackWords { get; set; }
        public long PC_LineNumber { get; set; }

        public AsmLabel(string Name, UInt32 StartAddr, string[] StackWords, long CharPosition, long LineNumber)
        {
            this.Name = Name;
            this.StartAddr = StartAddr;
            this.StackWords = StackWords;
            this.CharPosition = CharPosition;
            this.LineNumber = LineNumber;
        }

        public override string ToString()
        {
            return "<" + Name + ">   0x" + StartAddr.ToString("X") ;
        }
    }

    class DisassemblyManager
    {
        public string Filepath { get; set; }

        MemoryStream FileContents = null;
        StreamReader FileReader = null;

        Dictionary<string, AsmLabel> Labels = new Dictionary<string, AsmLabel>();
        
        public bool Load()
        {
            if (!File.Exists(Filepath))
                return false;

            try
            {
                if(FileContents!=null)
                {
                    FileReader.Dispose();
                    FileReader = null;

                    FileContents.Dispose();
                    FileContents = null;

                }

                FileContents = new MemoryStream(File.ReadAllBytes(Filepath));
                FileReader = new StreamReader(FileContents);

                return true;
            }
            catch(Exception e)
            {
                return false;
            }
        }

        static string[] watchLabels = new string[] { "ResetHandler", "main", "_Z9loopyloopmm" };

        public bool ParseLabels()
        {
            Labels = new Dictionary<string, AsmLabel>();

            FileReader.BaseStream.Seek(0, SeekOrigin.Begin);
            string line;
            long lineNumber = 0;
            long labelLineNumber = 0;

            int labelStartIndex;
            int labelEndIndex;

            long currentPos = 0;


            while(!FileReader.EndOfStream)
            {
                currentPos = GetActualPosition(FileReader);
                labelLineNumber = lineNumber;
                line = FileReader.ReadLine();
                lineNumber++;

                
                //currentPos = FileReader.BaseStream.Position;
                
                
                labelEndIndex = line.IndexOf(">:");
                //found a label!
                if(labelEndIndex!=-1)
                {
                    
                    labelStartIndex = line.LastIndexOf('<', labelEndIndex);
                    if(labelStartIndex == -1)
                    {
                        //unmatched brace
                    }
                    else
                    {
                        string LabelName = line.Substring(labelStartIndex + 1, labelEndIndex - labelStartIndex - 1);

                        if(Labels.ContainsKey(LabelName))
                        {
                            //duplicate label. Yes, these do exist. and are not supported by me.
                            continue;
                        }

                        string LabelAddrStr = line.Substring(0, labelStartIndex - 1);
                        UInt32 addr;
                        if(!UInt32.TryParse(LabelAddrStr, System.Globalization.NumberStyles.HexNumber, null, out addr))
                            continue;
                        
                        string tempLine;
                        
                        string[] regs=new string[]{};

                        for(int i=0;i<5;i++)
                        {
                            //char[] tempLineCharArray = new char[100];
                            //FileReader.BaseStream.Seek(currentPos, SeekOrigin.Begin);
                            //FileReader.Read(tempLineCharArray, 0, tempLineCharArray.Length);
                            //tempLine = new string(tempLineCharArray);
                            
                            tempLine = FileReader.ReadLine();
                            lineNumber++;
                            if (tempLine == null || tempLine.Contains(">:"))
                                break;
                            int startIndex = tempLine.IndexOf('{');
                            if(startIndex < 0)
                                continue;

                            int endIndex = tempLine.IndexOf('}', startIndex);
                            if (endIndex == -1)
                            {
                                string nextLine2 = FileReader.ReadLine();
                                lineNumber++;
                            }
                            else
                            {
                                string regsStr = tempLine.Substring(startIndex + 1, endIndex - startIndex - 1);
                                regs = regsStr.Split(new string[] { " ", ",", "\t" }, StringSplitOptions.RemoveEmptyEntries);
                            }
                            break;
                        }
                        //FileReader.BaseStream.Seek(currentPos, SeekOrigin.Begin);


                        if (LabelName == "main")
                            regs = new string[] { "", "", "" };
                        AsmLabel lbl = new AsmLabel(LabelName, addr, regs, currentPos, labelLineNumber);
                        
                        Labels.Add(LabelName, lbl);
                    }
                }

                
            }

            StringBuilder lineBuilder = new StringBuilder();
            return true;
        }

        /// <summary>
        /// Gets the label containing Addr
        /// </summary>
        /// <param name="Addr"></param>
        /// <returns>The label name</returns>
        public AsmLabel LookupLabel(UInt32 Addr)
        {
            if (FileContents == null)
                return null;

            int i;
            for (i = 0; i < Labels.Count; i++)
            {
                if (Labels.ElementAt(i).Value.StartAddr > Addr)
                    break;
            }

            return Labels.ElementAt(i-1).Value;
        }

        public void GetPC_LineNumber(AsmLabel lbl, UInt32 Addr)
        {
            UInt32 JAddr;
            if (Addr % 2 == 0)
                JAddr = Addr;
            else
                JAddr = Addr - 5;//all branch commands are off by 5. Not sure why

            lbl.PC_LineNumber = lbl.LineNumber;
            FileReader.DiscardBufferedData();
            FileReader.BaseStream.Seek(lbl.CharPosition, SeekOrigin.Begin);
            string line;
            int colonIndex;
            int openBraceIndex;


            while (true)
            {
                line = FileReader.ReadLine();
                lbl.PC_LineNumber++;
                if (FileReader.EndOfStream)
                    return;
                if (line == null || line.Length < 1)
                    continue;
                colonIndex = line.IndexOf(':');
                openBraceIndex = line.IndexOf('<');

                if (colonIndex < 0 || (openBraceIndex >= 0 && openBraceIndex < colonIndex))
                    continue;

                if (line.Contains("Disassembly of section .data"))
                    return;

                string addrStr = line.Substring(0, colonIndex).Trim();

                if (string.IsNullOrEmpty(addrStr))
                    return;

                UInt32 addr = UInt32.Parse(addrStr, System.Globalization.NumberStyles.HexNumber, null);

                if (addr == JAddr)
                {
                    int addrEnd = line.LastIndexOf('<');
                    if (addrEnd < 0)
                        return;
                    int addrStart = line.LastIndexOf('\t', addrEnd - 2);

                    if (addrStart > addrEnd)
                        return;

                    string jAddrStr = line.Substring(addrStart + 1, addrEnd - addrStart - 2);
                    UInt32 val = 0;
                    if (!UInt32.TryParse(jAddrStr, System.Globalization.NumberStyles.HexNumber, null, out val))
                    {
                        return;
                    }


                    return;
                }
                else if (addr > JAddr)
                    return;
            }


        }

        public AsmLabel GetJumpLabel(UInt32 Addr)
        {
            UInt32 JAddr;
            if(Addr %2 == 0)
                JAddr = Addr;
            else
                JAddr = Addr - 5;//all branch commands are off by 5. Not sure why
            AsmLabel containingCall = LookupLabel(Addr);

            FileReader.DiscardBufferedData();
            //long result = FileReader.BaseStream.Seek((long)((double)containingCall.LineNumber *.8) , SeekOrigin.Begin);
            long result = FileReader.BaseStream.Seek(containingCall.CharPosition, SeekOrigin.Begin);
            //FileReader.DiscardBufferedData();
            
            int colonIndex = 0;
            int openBraceIndex = 0;
            string line;

            while (true)
            {
                line = FileReader.ReadLine();
                if (FileReader.EndOfStream)
                    return null;
                if (line == null || line.Length < 1)
                    continue;
                colonIndex = line.IndexOf(':');
                openBraceIndex = line.IndexOf('<');

                if (colonIndex < 0 || (openBraceIndex >= 0 && openBraceIndex < colonIndex))
                    continue;

                if (line.Contains("Disassembly of section .data"))
                    return null;

                string addrStr = line.Substring(0, colonIndex).Trim();

                if (string.IsNullOrEmpty(addrStr))
                    return null;

                UInt32 addr = UInt32.Parse(addrStr, System.Globalization.NumberStyles.HexNumber, null);

                if (addr == JAddr)
                {
                    int addrEnd = line.LastIndexOf('<');
                    if(addrEnd < 0)
                        return null;
                    int addrStart = line.LastIndexOf('\t', addrEnd - 2);

                    if (addrStart > addrEnd)
                        return null;

                    string jAddrStr = line.Substring(addrStart + 1, addrEnd - addrStart - 2);
                    UInt32 val = 0;
                    if (!UInt32.TryParse(jAddrStr, System.Globalization.NumberStyles.HexNumber, null, out val))
                    {
                        return null;
                    }


                    return LookupLabel(val);
                }
                else if (addr > JAddr)
                    return null;
            }
            return null;
        }

        public static long GetActualPosition(StreamReader reader)
        {
            System.Reflection.BindingFlags flags = System.Reflection.BindingFlags.DeclaredOnly | System.Reflection.BindingFlags.NonPublic | System.Reflection.BindingFlags.Instance | System.Reflection.BindingFlags.GetField;

            // The current buffer of decoded characters
            char[] charBuffer = (char[])reader.GetType().InvokeMember("charBuffer", flags, null, reader, null);

            // The index of the next char to be read from charBuffer
            int charPos = (int)reader.GetType().InvokeMember("charPos", flags, null, reader, null);

            // The number of decoded chars presently used in charBuffer
            int charLen = (int)reader.GetType().InvokeMember("charLen", flags, null, reader, null);

            // The current buffer of read bytes (byteBuffer.Length = 1024; this is critical).
            byte[] byteBuffer = (byte[])reader.GetType().InvokeMember("byteBuffer", flags, null, reader, null);

            // The number of bytes read while advancing reader.BaseStream.Position to (re)fill charBuffer
            int byteLen = (int)reader.GetType().InvokeMember("byteLen", flags, null, reader, null);

            // The number of bytes the remaining chars use in the original encoding.
            int numBytesLeft = reader.CurrentEncoding.GetByteCount(charBuffer, charPos, charLen - charPos);

            // For variable-byte encodings, deal with partial chars at the end of the buffer
            int numFragments = 0;
            if (byteLen > 0 && !reader.CurrentEncoding.IsSingleByte)
            {
                if (reader.CurrentEncoding.CodePage == 65001) // UTF-8
                {
                    byte byteCountMask = 0;
                    while ((byteBuffer[byteLen - numFragments - 1] >> 6) == 2) // if the byte is "10xx xxxx", it's a continuation-byte
                        byteCountMask |= (byte)(1 << ++numFragments); // count bytes & build the "complete char" mask
                    if ((byteBuffer[byteLen - numFragments - 1] >> 6) == 3) // if the byte is "11xx xxxx", it starts a multi-byte char.
                        byteCountMask |= (byte)(1 << ++numFragments); // count bytes & build the "complete char" mask
                    // see if we found as many bytes as the leading-byte says to expect
                    if (numFragments > 1 && ((byteBuffer[byteLen - numFragments] >> 7 - numFragments) == byteCountMask))
                        numFragments = 0; // no partial-char in the byte-buffer to account for
                }
                else if (reader.CurrentEncoding.CodePage == 1200) // UTF-16LE
                {
                    if (byteBuffer[byteLen - 1] >= 0xd8) // high-surrogate
                        numFragments = 2; // account for the partial character
                }
                else if (reader.CurrentEncoding.CodePage == 1201) // UTF-16BE
                {
                    if (byteBuffer[byteLen - 2] >= 0xd8) // high-surrogate
                        numFragments = 2; // account for the partial character
                }
            }
            return reader.BaseStream.Position - numBytesLeft - numFragments;
        }
    }
}
