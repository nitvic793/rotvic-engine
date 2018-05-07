﻿// Code generated by Design interface, manipulated by Trevor Walden
namespace HUDEditor
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
            this.HUD_View = new System.Windows.Forms.GroupBox();
            this.imageEditPanel = new System.Windows.Forms.Panel();
            this.lockAspectBox = new System.Windows.Forms.CheckBox();
            this.heightBox = new System.Windows.Forms.NumericUpDown();
            this.label7 = new System.Windows.Forms.Label();
            this.label8 = new System.Windows.Forms.Label();
            this.widthBox = new System.Windows.Forms.NumericUpDown();
            this.changeTint = new System.Windows.Forms.Panel();
            this.label9 = new System.Windows.Forms.Label();
            this.changeImage = new System.Windows.Forms.TextBox();
            this.label10 = new System.Windows.Forms.Label();
            this.CreateElement = new System.Windows.Forms.Button();
            this.editLabel = new System.Windows.Forms.Label();
            this.exitButton = new System.Windows.Forms.Button();
            this.typeSelector = new System.Windows.Forms.ComboBox();
            this.exportButton = new System.Windows.Forms.Button();
            this.nameBox = new System.Windows.Forms.TextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.toolTip1 = new System.Windows.Forms.ToolTip(this.components);
            this.updateButton = new System.Windows.Forms.Button();
            this.label3 = new System.Windows.Forms.Label();
            this.nameChange = new System.Windows.Forms.TextBox();
            this.label6 = new System.Windows.Forms.Label();
            this.label5 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.colorPicker = new System.Windows.Forms.Panel();
            this.textChange = new System.Windows.Forms.TextBox();
            this.colorDialog = new System.Windows.Forms.ColorDialog();
            this.deleteButton = new System.Windows.Forms.Button();
            this.creationBox = new System.Windows.Forms.TextBox();
            this.creationLabel = new System.Windows.Forms.Label();
            this.openFileDialog = new System.Windows.Forms.OpenFileDialog();
            this.textEditPanel = new System.Windows.Forms.Panel();
            this.sizeChange = new System.Windows.Forms.NumericUpDown();
            this.imageEditPanel.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.heightBox)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.widthBox)).BeginInit();
            this.textEditPanel.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.sizeChange)).BeginInit();
            this.SuspendLayout();
            // 
            // HUD_View
            // 
            this.HUD_View.BackgroundImage = global::HUDEditor.Properties.Resources.GameScreen;
            this.HUD_View.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Stretch;
            this.HUD_View.Location = new System.Drawing.Point(5, 48);
            this.HUD_View.Margin = new System.Windows.Forms.Padding(2);
            this.HUD_View.Name = "HUD_View";
            this.HUD_View.Padding = new System.Windows.Forms.Padding(0);
            this.HUD_View.Size = new System.Drawing.Size(1280, 720);
            this.HUD_View.TabIndex = 0;
            this.HUD_View.TabStop = false;
            this.HUD_View.Text = "HUD View";
            this.toolTip1.SetToolTip(this.HUD_View, "This area represents the bounds of the screen in game");
            // 
            // imageEditPanel
            // 
            this.imageEditPanel.BackColor = System.Drawing.Color.Transparent;
            this.imageEditPanel.Controls.Add(this.lockAspectBox);
            this.imageEditPanel.Controls.Add(this.heightBox);
            this.imageEditPanel.Controls.Add(this.label7);
            this.imageEditPanel.Controls.Add(this.label8);
            this.imageEditPanel.Controls.Add(this.widthBox);
            this.imageEditPanel.Controls.Add(this.changeTint);
            this.imageEditPanel.Controls.Add(this.label9);
            this.imageEditPanel.Controls.Add(this.changeImage);
            this.imageEditPanel.Controls.Add(this.label10);
            this.imageEditPanel.Location = new System.Drawing.Point(594, 0);
            this.imageEditPanel.Name = "imageEditPanel";
            this.imageEditPanel.Size = new System.Drawing.Size(417, 48);
            this.imageEditPanel.TabIndex = 18;
            this.imageEditPanel.Visible = false;
            // 
            // lockAspectBox
            // 
            this.lockAspectBox.AutoSize = true;
            this.lockAspectBox.BackColor = System.Drawing.SystemColors.Control;
            this.lockAspectBox.Checked = true;
            this.lockAspectBox.CheckState = System.Windows.Forms.CheckState.Checked;
            this.lockAspectBox.Location = new System.Drawing.Point(310, 24);
            this.lockAspectBox.Name = "lockAspectBox";
            this.lockAspectBox.Size = new System.Drawing.Size(15, 14);
            this.lockAspectBox.TabIndex = 23;
            this.toolTip1.SetToolTip(this.lockAspectBox, "Scale width and height together");
            this.lockAspectBox.UseVisualStyleBackColor = false;
            // 
            // heightBox
            // 
            this.heightBox.DecimalPlaces = 2;
            this.heightBox.Increment = new decimal(new int[] {
            1,
            0,
            0,
            65536});
            this.heightBox.Location = new System.Drawing.Point(249, 22);
            this.heightBox.Maximum = new decimal(new int[] {
            150,
            0,
            0,
            0});
            this.heightBox.Name = "heightBox";
            this.heightBox.Size = new System.Drawing.Size(55, 20);
            this.heightBox.TabIndex = 22;
            this.toolTip1.SetToolTip(this.heightBox, "Edit the height of the selected element");
            this.heightBox.Value = new decimal(new int[] {
            5,
            0,
            0,
            0});
            this.heightBox.ValueChanged += new System.EventHandler(this.HeightBox_ValueChanged);
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(185, 7);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(81, 13);
            this.label7.TabIndex = 21;
            this.label7.Text = "Edit Scale- X,Y:";
            this.toolTip1.SetToolTip(this.label7, "Edit the size of the selected element");
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.Location = new System.Drawing.Point(133, 7);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(49, 13);
            this.label8.TabIndex = 20;
            this.label8.Text = "Edit Tint:";
            this.toolTip1.SetToolTip(this.label8, "Edit the tint of the selected element");
            // 
            // widthBox
            // 
            this.widthBox.DecimalPlaces = 2;
            this.widthBox.Increment = new decimal(new int[] {
            1,
            0,
            0,
            65536});
            this.widthBox.Location = new System.Drawing.Point(188, 22);
            this.widthBox.Maximum = new decimal(new int[] {
            150,
            0,
            0,
            0});
            this.widthBox.Name = "widthBox";
            this.widthBox.Size = new System.Drawing.Size(55, 20);
            this.widthBox.TabIndex = 19;
            this.toolTip1.SetToolTip(this.widthBox, "Edit the width of the selected element");
            this.widthBox.Value = new decimal(new int[] {
            5,
            0,
            0,
            0});
            this.widthBox.ValueChanged += new System.EventHandler(this.WidthBox_ValueChanged);
            // 
            // changeTint
            // 
            this.changeTint.BackColor = System.Drawing.Color.Black;
            this.changeTint.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.changeTint.Location = new System.Drawing.Point(136, 23);
            this.changeTint.Name = "changeTint";
            this.changeTint.Size = new System.Drawing.Size(46, 20);
            this.changeTint.TabIndex = 18;
            this.toolTip1.SetToolTip(this.changeTint, "Edit the tint of the selected element");
            // 
            // label9
            // 
            this.label9.AutoSize = true;
            this.label9.Location = new System.Drawing.Point(3, 7);
            this.label9.Name = "label9";
            this.label9.Size = new System.Drawing.Size(76, 13);
            this.label9.TabIndex = 15;
            this.label9.Text = "Change Image";
            this.toolTip1.SetToolTip(this.label9, "Change the image of the selected element");
            // 
            // changeImage
            // 
            this.changeImage.Location = new System.Drawing.Point(6, 23);
            this.changeImage.Name = "changeImage";
            this.changeImage.Size = new System.Drawing.Size(124, 20);
            this.changeImage.TabIndex = 11;
            this.toolTip1.SetToolTip(this.changeImage, "Change the image of the selected element (this is the string used as its key in t" +
        "he engine\'s UI map)");
            this.changeImage.Click += new System.EventHandler(this.ChangeImage_Click);
            // 
            // label10
            // 
            this.label10.Font = new System.Drawing.Font("Microsoft Sans Serif", 6F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label10.Location = new System.Drawing.Point(291, 4);
            this.label10.Name = "label10";
            this.label10.Size = new System.Drawing.Size(52, 21);
            this.label10.TabIndex = 24;
            this.label10.Text = "Lock Aspect Ratio";
            this.label10.TextAlign = System.Drawing.ContentAlignment.TopCenter;
            this.toolTip1.SetToolTip(this.label10, "Scale width and height together");
            // 
            // CreateElement
            // 
            this.CreateElement.Location = new System.Drawing.Point(323, 7);
            this.CreateElement.Margin = new System.Windows.Forms.Padding(2);
            this.CreateElement.Name = "CreateElement";
            this.CreateElement.Size = new System.Drawing.Size(53, 36);
            this.CreateElement.TabIndex = 1;
            this.CreateElement.Text = "Create Element";
            this.toolTip1.SetToolTip(this.CreateElement, "Create the element with the given parameters");
            this.CreateElement.UseVisualStyleBackColor = true;
            this.CreateElement.Click += new System.EventHandler(this.CreateElement_Click);
            // 
            // editLabel
            // 
            this.editLabel.AutoEllipsis = true;
            this.editLabel.Cursor = System.Windows.Forms.Cursors.Default;
            this.editLabel.Location = new System.Drawing.Point(376, 26);
            this.editLabel.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
            this.editLabel.Name = "editLabel";
            this.editLabel.Size = new System.Drawing.Size(120, 13);
            this.editLabel.TabIndex = 2;
            this.editLabel.Text = "Edit []:";
            // 
            // exitButton
            // 
            this.exitButton.Location = new System.Drawing.Point(1220, 7);
            this.exitButton.Name = "exitButton";
            this.exitButton.Size = new System.Drawing.Size(60, 38);
            this.exitButton.TabIndex = 3;
            this.exitButton.Text = "Exit";
            this.toolTip1.SetToolTip(this.exitButton, "Exit the editor");
            this.exitButton.UseVisualStyleBackColor = true;
            this.exitButton.Click += new System.EventHandler(this.ExitButton_Click);
            // 
            // typeSelector
            // 
            this.typeSelector.FormattingEnabled = true;
            this.typeSelector.Items.AddRange(new object[] {
            "Text",
            "Image"});
            this.typeSelector.Location = new System.Drawing.Point(5, 22);
            this.typeSelector.Name = "typeSelector";
            this.typeSelector.Size = new System.Drawing.Size(72, 21);
            this.typeSelector.TabIndex = 4;
            this.toolTip1.SetToolTip(this.typeSelector, "Whether this UI element is text, or an image");
            this.typeSelector.SelectedIndexChanged += new System.EventHandler(this.TypeSelected);
            // 
            // exportButton
            // 
            this.exportButton.Location = new System.Drawing.Point(1154, 7);
            this.exportButton.Name = "exportButton";
            this.exportButton.Size = new System.Drawing.Size(60, 38);
            this.exportButton.TabIndex = 5;
            this.exportButton.Text = "Export to engine";
            this.toolTip1.SetToolTip(this.exportButton, "Export the layout you\'ve set up to be used in the engine");
            this.exportButton.UseVisualStyleBackColor = true;
            this.exportButton.Click += new System.EventHandler(this.ExportButton_Click);
            // 
            // nameBox
            // 
            this.nameBox.Location = new System.Drawing.Point(83, 23);
            this.nameBox.Name = "nameBox";
            this.nameBox.Size = new System.Drawing.Size(111, 20);
            this.nameBox.TabIndex = 6;
            this.toolTip1.SetToolTip(this.nameBox, "This string will be used as the UI element\'s identifier in the engine\'s UI map");
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(2, 7);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(75, 13);
            this.label1.TabIndex = 7;
            this.label1.Text = "Element Type:";
            this.toolTip1.SetToolTip(this.label1, "Whether this UI element is text, or an image");
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(80, 7);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(79, 13);
            this.label2.TabIndex = 8;
            this.label2.Text = "Element Name:";
            this.toolTip1.SetToolTip(this.label2, "This string will be used as the UI element\'s identifier in the engine\'s UI map");
            // 
            // updateButton
            // 
            this.updateButton.Location = new System.Drawing.Point(1022, 7);
            this.updateButton.Name = "updateButton";
            this.updateButton.Size = new System.Drawing.Size(60, 38);
            this.updateButton.TabIndex = 14;
            this.updateButton.Text = "Update Element";
            this.toolTip1.SetToolTip(this.updateButton, "Update the selected element with the provided parameters");
            this.updateButton.UseVisualStyleBackColor = true;
            this.updateButton.Click += new System.EventHandler(this.UpdateButton_Click);
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(495, 7);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(59, 13);
            this.label3.TabIndex = 19;
            this.label3.Text = "Edit Name:";
            this.toolTip1.SetToolTip(this.label3, "Edit the name of the selected element (this is the string used as its key in the " +
        "engine\'s UI map)");
            // 
            // nameChange
            // 
            this.nameChange.Location = new System.Drawing.Point(498, 23);
            this.nameChange.Name = "nameChange";
            this.nameChange.Size = new System.Drawing.Size(97, 20);
            this.nameChange.TabIndex = 18;
            this.toolTip1.SetToolTip(this.nameChange, "Edit the name of the selected element (this is the string used as its key in the " +
        "engine\'s UI map)");
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(194, 7);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(51, 13);
            this.label6.TabIndex = 17;
            this.label6.Text = "Edit Size:";
            this.toolTip1.SetToolTip(this.label6, "Edit the font size of the selected element");
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(136, 7);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(55, 13);
            this.label5.TabIndex = 16;
            this.label5.Text = "Edit Color:";
            this.toolTip1.SetToolTip(this.label5, "Edit the font color of the selected element");
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(3, 7);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(52, 13);
            this.label4.TabIndex = 15;
            this.label4.Text = "Edit Text:";
            this.toolTip1.SetToolTip(this.label4, "Edit the text of the selected element");
            // 
            // colorPicker
            // 
            this.colorPicker.BackColor = System.Drawing.Color.Black;
            this.colorPicker.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.colorPicker.Location = new System.Drawing.Point(139, 23);
            this.colorPicker.Name = "colorPicker";
            this.colorPicker.Size = new System.Drawing.Size(52, 20);
            this.colorPicker.TabIndex = 12;
            this.toolTip1.SetToolTip(this.colorPicker, "Edit the font color of the selected element");
            // 
            // textChange
            // 
            this.textChange.Location = new System.Drawing.Point(6, 23);
            this.textChange.Name = "textChange";
            this.textChange.Size = new System.Drawing.Size(124, 20);
            this.textChange.TabIndex = 11;
            this.toolTip1.SetToolTip(this.textChange, "Edit the text of the selected element (this is the string used as its key in the " +
        "engine\'s UI map)");
            // 
            // colorDialog
            // 
            this.colorDialog.ShowHelp = true;
            // 
            // deleteButton
            // 
            this.deleteButton.Location = new System.Drawing.Point(1088, 7);
            this.deleteButton.Name = "deleteButton";
            this.deleteButton.Size = new System.Drawing.Size(60, 38);
            this.deleteButton.TabIndex = 15;
            this.deleteButton.Text = "Delete Element";
            this.deleteButton.UseVisualStyleBackColor = true;
            this.deleteButton.Click += new System.EventHandler(this.DeleteButton_Click);
            // 
            // creationBox
            // 
            this.creationBox.Location = new System.Drawing.Point(200, 23);
            this.creationBox.Name = "creationBox";
            this.creationBox.Size = new System.Drawing.Size(118, 20);
            this.creationBox.TabIndex = 16;
            this.creationBox.Click += new System.EventHandler(this.CreationBox_Click);
            // 
            // creationLabel
            // 
            this.creationLabel.AutoSize = true;
            this.creationLabel.Location = new System.Drawing.Point(197, 7);
            this.creationLabel.Name = "creationLabel";
            this.creationLabel.Size = new System.Drawing.Size(72, 13);
            this.creationLabel.TabIndex = 17;
            this.creationLabel.Text = "Element Text:";
            // 
            // openFileDialog
            // 
            this.openFileDialog.FileName = "image.png";
            this.openFileDialog.Filter = "Image Files(*.png;*.PNG;*.jpg,*.JPG,*.jpeg,*.JPEG,*.bmp,*.BMP)|*.png;*.PNG;*.jpg," +
    "*.JPG,*.jpeg,*.JPEG,*.bmp,*.BMP";
            this.openFileDialog.RestoreDirectory = true;
            // 
            // textEditPanel
            // 
            this.textEditPanel.BackColor = System.Drawing.Color.Transparent;
            this.textEditPanel.Controls.Add(this.label6);
            this.textEditPanel.Controls.Add(this.label5);
            this.textEditPanel.Controls.Add(this.label4);
            this.textEditPanel.Controls.Add(this.sizeChange);
            this.textEditPanel.Controls.Add(this.colorPicker);
            this.textEditPanel.Controls.Add(this.textChange);
            this.textEditPanel.Location = new System.Drawing.Point(595, 0);
            this.textEditPanel.Name = "textEditPanel";
            this.textEditPanel.Size = new System.Drawing.Size(377, 48);
            this.textEditPanel.TabIndex = 20;
            this.textEditPanel.Visible = false;
            // 
            // sizeChange
            // 
            this.sizeChange.DecimalPlaces = 2;
            this.sizeChange.Location = new System.Drawing.Point(197, 24);
            this.sizeChange.Minimum = new decimal(new int[] {
            5,
            0,
            0,
            0});
            this.sizeChange.Name = "sizeChange";
            this.sizeChange.Size = new System.Drawing.Size(55, 20);
            this.sizeChange.TabIndex = 13;
            this.sizeChange.Value = new decimal(new int[] {
            5,
            0,
            0,
            0});
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1292, 773);
            this.Controls.Add(this.creationLabel);
            this.Controls.Add(this.creationBox);
            this.Controls.Add(this.deleteButton);
            this.Controls.Add(this.updateButton);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.nameBox);
            this.Controls.Add(this.exportButton);
            this.Controls.Add(this.typeSelector);
            this.Controls.Add(this.exitButton);
            this.Controls.Add(this.editLabel);
            this.Controls.Add(this.CreateElement);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.nameChange);
            this.Controls.Add(this.imageEditPanel);
            this.Controls.Add(this.textEditPanel);
            this.Controls.Add(this.HUD_View);
            this.Margin = new System.Windows.Forms.Padding(2);
            this.Name = "Form1";
            this.Text = "Form1";
            this.Load += new System.EventHandler(this.Form1_Load);
            this.imageEditPanel.ResumeLayout(false);
            this.imageEditPanel.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.heightBox)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.widthBox)).EndInit();
            this.textEditPanel.ResumeLayout(false);
            this.textEditPanel.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.sizeChange)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.GroupBox HUD_View;
        private System.Windows.Forms.Button CreateElement;
        private System.Windows.Forms.Label editLabel;
        private System.Windows.Forms.Button exitButton;
        private System.Windows.Forms.ComboBox typeSelector;
        private System.Windows.Forms.Button exportButton;
        private System.Windows.Forms.TextBox nameBox;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.ToolTip toolTip1;
        private System.Windows.Forms.ColorDialog colorDialog;
        private System.Windows.Forms.Button updateButton;
        private System.Windows.Forms.Button deleteButton;
        private System.Windows.Forms.TextBox creationBox;
        private System.Windows.Forms.Label creationLabel;
        private System.Windows.Forms.OpenFileDialog openFileDialog;
        private System.Windows.Forms.Panel imageEditPanel;
        private System.Windows.Forms.Label label9;
        private System.Windows.Forms.TextBox changeImage;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.TextBox nameChange;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.NumericUpDown widthBox;
        private System.Windows.Forms.Panel changeTint;
        private System.Windows.Forms.CheckBox lockAspectBox;
        private System.Windows.Forms.NumericUpDown heightBox;
        private System.Windows.Forms.Label label10;
        private System.Windows.Forms.Panel textEditPanel;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.NumericUpDown sizeChange;
        private System.Windows.Forms.Panel colorPicker;
        private System.Windows.Forms.TextBox textChange;
    }
}

