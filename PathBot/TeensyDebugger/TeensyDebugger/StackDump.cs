using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Diagnostics;

namespace TeensyDebugger
{
    class StackDump
    {
        public UInt32[] Data { get; protected set; }

        public static int offset = -2;
        public static int exectedLength = 32;

        DisassemblyManager Manager;

        public StackDump(string input, DisassemblyManager Manager)
        {
            this.Manager = Manager;
            string[] words = input.Split(' ');
            Data = new UInt32[words.Length];
            for(int i=0;i<words.Length;i++)
            {
                UInt32.TryParse(words[i], System.Globalization.NumberStyles.HexNumber,  null, out Data[i]);
            }

            if (Data.Length != 32)
                Debug.Print("Expected 32 words. Got " + Data.Length);

    
        }

        enum Region
        {
            Undefined,
            Stack,
            ISRStack

        }

        public Stack<AsmLabel> Unwind()
        {
            int index = Data.Length - 1;
            Stack<AsmLabel> result = new Stack<AsmLabel>();
            bool frontMatter = true;
            bool stack = false;
            bool ISRStack = false;

            UInt32 cWord;

            while(index >=0)
            {
                cWord = Data[index];
                if (frontMatter)
                {
                    if (cWord != 0xffffffff && cWord != 0x00000000)
                    {
                        frontMatter = false;
                        stack = true;
                        continue;
                    }
                    else
                    {
                        index--;
                    }
                }
                else if(stack)
                {
                    if (cWord > 0x10000000)
                    {
                        index--;
                        continue;
                    }
                    if (cWord == 0xffffffff && cWord == 0x00000000)
                    {
                        stack = false;
                        ISRStack = true;
                        index--;
                        continue;
                    }
                    AsmLabel lbl = Manager.LookupLabel(cWord);
                    AsmLabel jumpLbl = Manager.GetJumpLabel(cWord);
                    Manager.GetPC_LineNumber(lbl, cWord);

                    if(jumpLbl == null)
                    {
                        stack = false;
                        ISRStack = true;
                        index--;
                        continue;
                    }

                    if (lbl != null)
                    {
                        if (jumpLbl != null)
                            index -= (jumpLbl.StackWords.Length> 0 ? jumpLbl.StackWords.Length :1);
                        else
                            index--;
                        result.Push(lbl);
                    }
                    else
                    {
                        stack = false;
                        ISRStack = true;
                        index--;
                    }
                }
                else if(ISRStack)
                {
                    index--;
                }
                if (index < 0)
                    break;
            }
            return result;

        }
    }
}
