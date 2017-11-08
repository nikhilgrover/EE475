namespace TeensyDebugger
{
    partial class StackControl
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

        #region Component Designer generated code

        /// <summary> 
        /// Required method for Designer support - do not modify 
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.lnkLbl = new System.Windows.Forms.LinkLabel();
            this.lblLabel = new System.Windows.Forms.Label();
            this.SuspendLayout();
            // 
            // lnkLbl
            // 
            this.lnkLbl.AutoSize = true;
            this.lnkLbl.Location = new System.Drawing.Point(3, 13);
            this.lnkLbl.Name = "lnkLbl";
            this.lnkLbl.Size = new System.Drawing.Size(220, 13);
            this.lnkLbl.TabIndex = 0;
            this.lnkLbl.TabStop = true;
            this.lnkLbl.Text = "C:\\Teensy\\Projects\\HelloWorld\\main.cpp:23";
            this.lnkLbl.LinkClicked += new System.Windows.Forms.LinkLabelLinkClickedEventHandler(this.lnkLbl_LinkClicked);
            // 
            // lblLabel
            // 
            this.lblLabel.AutoSize = true;
            this.lblLabel.Location = new System.Drawing.Point(3, 0);
            this.lblLabel.Name = "lblLabel";
            this.lblLabel.Size = new System.Drawing.Size(75, 13);
            this.lblLabel.TabIndex = 1;
            this.lblLabel.Text = "main()   line 23";
            // 
            // StackControl
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.lblLabel);
            this.Controls.Add(this.lnkLbl);
            this.Name = "StackControl";
            this.Size = new System.Drawing.Size(229, 28);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.LinkLabel lnkLbl;
        private System.Windows.Forms.Label lblLabel;
    }
}
