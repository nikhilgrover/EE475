namespace TeensyDebugger
{
    partial class Form1
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            this.txtbxOutput = new FastColoredTextBoxNS.FastColoredTextBox();
            this.btnBreak = new System.Windows.Forms.Button();
            this.btnCompile = new System.Windows.Forms.Button();
            this.btnClear = new System.Windows.Forms.Button();
            this.btnRestart = new System.Windows.Forms.Button();
            this.txtbxCmd = new System.Windows.Forms.TextBox();
            this.btnStop = new System.Windows.Forms.Button();
            this.splitContainer1 = new System.Windows.Forms.SplitContainer();
            this.stackFlowLayout1 = new TeensyDebugger.StackFlowLayout(this.components);
            ((System.ComponentModel.ISupportInitialize)(this.txtbxOutput)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.splitContainer1)).BeginInit();
            this.splitContainer1.Panel1.SuspendLayout();
            this.splitContainer1.Panel2.SuspendLayout();
            this.splitContainer1.SuspendLayout();
            this.SuspendLayout();
            // 
            // txtbxOutput
            // 
            this.txtbxOutput.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.txtbxOutput.AutoCompleteBracketsList = new char[] {
        '(',
        ')',
        '{',
        '}',
        '[',
        ']',
        '\"',
        '\"',
        '\'',
        '\''};
            this.txtbxOutput.AutoScrollMinSize = new System.Drawing.Size(0, 18);
            this.txtbxOutput.BackBrush = null;
            this.txtbxOutput.CharHeight = 18;
            this.txtbxOutput.CharWidth = 9;
            this.txtbxOutput.Cursor = System.Windows.Forms.Cursors.IBeam;
            this.txtbxOutput.DisabledColor = System.Drawing.Color.FromArgb(((int)(((byte)(100)))), ((int)(((byte)(180)))), ((int)(((byte)(180)))), ((int)(((byte)(180)))));
            this.txtbxOutput.Font = new System.Drawing.Font("Consolas", 12F);
            this.txtbxOutput.IsReplaceMode = false;
            this.txtbxOutput.Location = new System.Drawing.Point(0, 0);
            this.txtbxOutput.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.txtbxOutput.Name = "txtbxOutput";
            this.txtbxOutput.Paddings = new System.Windows.Forms.Padding(0);
            this.txtbxOutput.ReadOnly = true;
            this.txtbxOutput.SelectionColor = System.Drawing.Color.FromArgb(((int)(((byte)(60)))), ((int)(((byte)(0)))), ((int)(((byte)(0)))), ((int)(((byte)(255)))));
            this.txtbxOutput.ShowLineNumbers = false;
            this.txtbxOutput.Size = new System.Drawing.Size(976, 405);
            this.txtbxOutput.TabIndex = 0;
            this.txtbxOutput.WordWrap = true;
            this.txtbxOutput.Zoom = 100;
            // 
            // btnBreak
            // 
            this.btnBreak.Location = new System.Drawing.Point(333, 18);
            this.btnBreak.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.btnBreak.Name = "btnBreak";
            this.btnBreak.Size = new System.Drawing.Size(137, 43);
            this.btnBreak.TabIndex = 1;
            this.btnBreak.Text = "Get Call Stack";
            this.btnBreak.UseVisualStyleBackColor = true;
            this.btnBreak.Click += new System.EventHandler(this.btnBreak_Click);
            // 
            // btnCompile
            // 
            this.btnCompile.Location = new System.Drawing.Point(245, 18);
            this.btnCompile.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.btnCompile.Name = "btnCompile";
            this.btnCompile.Size = new System.Drawing.Size(80, 43);
            this.btnCompile.TabIndex = 2;
            this.btnCompile.Text = "Compile";
            this.btnCompile.UseVisualStyleBackColor = true;
            this.btnCompile.Click += new System.EventHandler(this.btnCompile_Click);
            // 
            // btnClear
            // 
            this.btnClear.Location = new System.Drawing.Point(18, 18);
            this.btnClear.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.btnClear.Name = "btnClear";
            this.btnClear.Size = new System.Drawing.Size(65, 43);
            this.btnClear.TabIndex = 4;
            this.btnClear.Text = "Clear";
            this.btnClear.UseVisualStyleBackColor = true;
            this.btnClear.Click += new System.EventHandler(this.btnClear_Click);
            // 
            // btnRestart
            // 
            this.btnRestart.Location = new System.Drawing.Point(158, 18);
            this.btnRestart.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.btnRestart.Name = "btnRestart";
            this.btnRestart.Size = new System.Drawing.Size(79, 43);
            this.btnRestart.TabIndex = 5;
            this.btnRestart.Text = "Restart";
            this.btnRestart.UseVisualStyleBackColor = true;
            this.btnRestart.Click += new System.EventHandler(this.btnRestart_Click);
            // 
            // txtbxCmd
            // 
            this.txtbxCmd.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.txtbxCmd.Location = new System.Drawing.Point(0, 6);
            this.txtbxCmd.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.txtbxCmd.Multiline = true;
            this.txtbxCmd.Name = "txtbxCmd";
            this.txtbxCmd.Size = new System.Drawing.Size(976, 44);
            this.txtbxCmd.TabIndex = 6;
            this.txtbxCmd.KeyDown += new System.Windows.Forms.KeyEventHandler(this.txtbxCmd_KeyDown);
            // 
            // btnStop
            // 
            this.btnStop.Location = new System.Drawing.Point(91, 18);
            this.btnStop.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.btnStop.Name = "btnStop";
            this.btnStop.Size = new System.Drawing.Size(59, 43);
            this.btnStop.TabIndex = 7;
            this.btnStop.Text = "Stop";
            this.btnStop.UseVisualStyleBackColor = true;
            this.btnStop.Click += new System.EventHandler(this.btnStop_Click);
            // 
            // splitContainer1
            // 
            this.splitContainer1.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.splitContainer1.Location = new System.Drawing.Point(0, 61);
            this.splitContainer1.Name = "splitContainer1";
            this.splitContainer1.Orientation = System.Windows.Forms.Orientation.Horizontal;
            // 
            // splitContainer1.Panel1
            // 
            this.splitContainer1.Panel1.Controls.Add(this.txtbxOutput);
            // 
            // splitContainer1.Panel2
            // 
            this.splitContainer1.Panel2.Controls.Add(this.txtbxCmd);
            this.splitContainer1.Panel2.Controls.Add(this.stackFlowLayout1);
            this.splitContainer1.Size = new System.Drawing.Size(976, 579);
            this.splitContainer1.SplitterDistance = 405;
            this.splitContainer1.TabIndex = 10;
            // 
            // stackFlowLayout1
            // 
            this.stackFlowLayout1.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.stackFlowLayout1.AutoScroll = true;
            this.stackFlowLayout1.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.stackFlowLayout1.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.stackFlowLayout1.Location = new System.Drawing.Point(2, 50);
            this.stackFlowLayout1.Margin = new System.Windows.Forms.Padding(2);
            this.stackFlowLayout1.Name = "stackFlowLayout1";
            this.stackFlowLayout1.Size = new System.Drawing.Size(972, 118);
            this.stackFlowLayout1.TabIndex = 9;
            this.stackFlowLayout1.WrapContents = false;
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(9F, 20F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(976, 640);
            this.Controls.Add(this.splitContainer1);
            this.Controls.Add(this.btnStop);
            this.Controls.Add(this.btnRestart);
            this.Controls.Add(this.btnClear);
            this.Controls.Add(this.btnCompile);
            this.Controls.Add(this.btnBreak);
            this.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.Name = "Form1";
            this.Text = "Form1";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.Form1_FormClosing);
            this.Load += new System.EventHandler(this.Form1_Load);
            ((System.ComponentModel.ISupportInitialize)(this.txtbxOutput)).EndInit();
            this.splitContainer1.Panel1.ResumeLayout(false);
            this.splitContainer1.Panel2.ResumeLayout(false);
            this.splitContainer1.Panel2.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.splitContainer1)).EndInit();
            this.splitContainer1.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private FastColoredTextBoxNS.FastColoredTextBox txtbxOutput;
        private System.Windows.Forms.Button btnBreak;
        private System.Windows.Forms.Button btnCompile;
        private System.Windows.Forms.Button btnClear;
        private System.Windows.Forms.Button btnRestart;
        private System.Windows.Forms.TextBox txtbxCmd;
        private System.Windows.Forms.Button btnStop;
        private StackFlowLayout stackFlowLayout1;
        private System.Windows.Forms.SplitContainer splitContainer1;

    }
}

