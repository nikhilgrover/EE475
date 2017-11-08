using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace TeensyDebugger
{
    public partial class StackFlowLayout : FlowLayoutPanel
    {
        public StackFlowLayout()
        {
            InitializeComponent();
        }

        public StackFlowLayout(IContainer container)
        {
            container.Add(this);

            InitializeComponent();
            this.WrapContents = false;
            this.FlowDirection = System.Windows.Forms.FlowDirection.TopDown;
            this.AutoScroll = true;
        }


        [DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden)]
        public PacketStateMachine Server { get; set; }


        List<StackControl> ctrls = new List<StackControl>();

        public void UpdateStack(Stack<AsmLabel> stack)
        {
            this.Controls.Clear();

            //copy stack to an iterable array
            AsmLabel[] stackarray = new AsmLabel[stack.Count];
            stack.CopyTo(stackarray, 0);

            //print data to GUI
            foreach (AsmLabel lbl in stackarray)
            {
                StackControl ctrl = new StackControl();
                ctrl.Server = this.Server;
                ctrl.Filepath = "C:\\Teensy\\Projects\\HelloWorld\\Main_Asm.txt";
                ctrl.Line = lbl.PC_LineNumber;
                ctrl.Label = lbl.Name;
                ctrl.Width = this.ClientRectangle.Width/2;
                ctrl.Margin = new Padding(0, 0, 0, 0);
                this.Controls.Add(ctrl);
            }
        }

    }
}
