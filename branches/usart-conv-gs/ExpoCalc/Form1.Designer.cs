namespace ExpoCalc
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
            this.nudInputLower = new System.Windows.Forms.NumericUpDown();
            this.nudInputHigher = new System.Windows.Forms.NumericUpDown();
            this.nudOutputLower = new System.Windows.Forms.NumericUpDown();
            this.nudOutputHigher = new System.Windows.Forms.NumericUpDown();
            this.buttonCalculate = new System.Windows.Forms.Button();
            this.lbOutput = new System.Windows.Forms.ListBox();
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.nudExpo = new System.Windows.Forms.NumericUpDown();
            this.label3 = new System.Windows.Forms.Label();
            ((System.ComponentModel.ISupportInitialize)(this.nudInputLower)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.nudInputHigher)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.nudOutputLower)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.nudOutputHigher)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.nudExpo)).BeginInit();
            this.SuspendLayout();
            // 
            // nudInputLower
            // 
            this.nudInputLower.Location = new System.Drawing.Point(224, 14);
            this.nudInputLower.Maximum = new decimal(new int[] {
            5000,
            0,
            0,
            0});
            this.nudInputLower.Minimum = new decimal(new int[] {
            5000,
            0,
            0,
            -2147483648});
            this.nudInputLower.Name = "nudInputLower";
            this.nudInputLower.Size = new System.Drawing.Size(47, 20);
            this.nudInputLower.TabIndex = 0;
            this.nudInputLower.Value = new decimal(new int[] {
            100,
            0,
            0,
            -2147483648});
            // 
            // nudInputHigher
            // 
            this.nudInputHigher.Location = new System.Drawing.Point(313, 13);
            this.nudInputHigher.Maximum = new decimal(new int[] {
            5000,
            0,
            0,
            0});
            this.nudInputHigher.Minimum = new decimal(new int[] {
            5000,
            0,
            0,
            -2147483648});
            this.nudInputHigher.Name = "nudInputHigher";
            this.nudInputHigher.Size = new System.Drawing.Size(47, 20);
            this.nudInputHigher.TabIndex = 0;
            this.nudInputHigher.Value = new decimal(new int[] {
            100,
            0,
            0,
            0});
            // 
            // nudOutputLower
            // 
            this.nudOutputLower.Location = new System.Drawing.Point(224, 40);
            this.nudOutputLower.Maximum = new decimal(new int[] {
            5000,
            0,
            0,
            0});
            this.nudOutputLower.Minimum = new decimal(new int[] {
            5000,
            0,
            0,
            -2147483648});
            this.nudOutputLower.Name = "nudOutputLower";
            this.nudOutputLower.Size = new System.Drawing.Size(47, 20);
            this.nudOutputLower.TabIndex = 0;
            this.nudOutputLower.Value = new decimal(new int[] {
            700,
            0,
            0,
            0});
            // 
            // nudOutputHigher
            // 
            this.nudOutputHigher.Location = new System.Drawing.Point(313, 39);
            this.nudOutputHigher.Maximum = new decimal(new int[] {
            5000,
            0,
            0,
            0});
            this.nudOutputHigher.Minimum = new decimal(new int[] {
            5000,
            0,
            0,
            -2147483648});
            this.nudOutputHigher.Name = "nudOutputHigher";
            this.nudOutputHigher.Size = new System.Drawing.Size(47, 20);
            this.nudOutputHigher.TabIndex = 0;
            this.nudOutputHigher.Value = new decimal(new int[] {
            1700,
            0,
            0,
            0});
            // 
            // buttonCalculate
            // 
            this.buttonCalculate.Location = new System.Drawing.Point(9, 97);
            this.buttonCalculate.Name = "buttonCalculate";
            this.buttonCalculate.Size = new System.Drawing.Size(351, 36);
            this.buttonCalculate.TabIndex = 1;
            this.buttonCalculate.Text = "Calculate";
            this.buttonCalculate.UseVisualStyleBackColor = true;
            this.buttonCalculate.Click += new System.EventHandler(this.buttonCalculate_Click);
            // 
            // lbOutput
            // 
            this.lbOutput.FormattingEnabled = true;
            this.lbOutput.Location = new System.Drawing.Point(9, 139);
            this.lbOutput.Name = "lbOutput";
            this.lbOutput.Size = new System.Drawing.Size(351, 290);
            this.lbOutput.TabIndex = 2;
            // 
            // label1
            // 
            this.label1.Location = new System.Drawing.Point(12, 13);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(194, 27);
            this.label1.TabIndex = 3;
            this.label1.Text = "Input Range (stick position/signed percentage*)";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(12, 46);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(194, 13);
            this.label2.TabIndex = 3;
            this.label2.Text = "Output Range (servo ms*/arbitrary/adc)";
            // 
            // nudExpo
            // 
            this.nudExpo.Location = new System.Drawing.Point(313, 65);
            this.nudExpo.Maximum = new decimal(new int[] {
            5000,
            0,
            0,
            0});
            this.nudExpo.Minimum = new decimal(new int[] {
            5000,
            0,
            0,
            -2147483648});
            this.nudExpo.Name = "nudExpo";
            this.nudExpo.Size = new System.Drawing.Size(47, 20);
            this.nudExpo.TabIndex = 0;
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(12, 71);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(31, 13);
            this.label3.TabIndex = 3;
            this.label3.Text = "Expo";
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(368, 442);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.lbOutput);
            this.Controls.Add(this.buttonCalculate);
            this.Controls.Add(this.nudExpo);
            this.Controls.Add(this.nudOutputHigher);
            this.Controls.Add(this.nudOutputLower);
            this.Controls.Add(this.nudInputHigher);
            this.Controls.Add(this.nudInputLower);
            this.Name = "Form1";
            this.Text = "Exp Calculator";
            this.Load += new System.EventHandler(this.Form1_Load);
            ((System.ComponentModel.ISupportInitialize)(this.nudInputLower)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.nudInputHigher)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.nudOutputLower)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.nudOutputHigher)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.nudExpo)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.NumericUpDown nudInputLower;
        private System.Windows.Forms.NumericUpDown nudInputHigher;
        private System.Windows.Forms.NumericUpDown nudOutputLower;
        private System.Windows.Forms.NumericUpDown nudOutputHigher;
        private System.Windows.Forms.Button buttonCalculate;
        private System.Windows.Forms.ListBox lbOutput;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.NumericUpDown nudExpo;
        private System.Windows.Forms.Label label3;
    }
}

