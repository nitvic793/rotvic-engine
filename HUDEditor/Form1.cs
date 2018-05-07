using System;
using System.Drawing;
using System.IO;
using System.Windows.Forms;
// Code written by Trevor Walden with reference from MSDN documentation
namespace HUDEditor
{
    public partial class Form1 : Form // MSDN documentation for windows forms used as reference
    {
        public Form1()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            colorPicker.Click += delegate
            {
                if (colorDialog.ShowDialog() == DialogResult.OK)
                {
                    colorPicker.BackColor = colorDialog.Color;
                }
            };

            updateButton.Click += delegate {
                if (MessageBox.Show("Are you sure you wish to update this element? You cannot undo this.", "Update Element", MessageBoxButtons.YesNo) == DialogResult.Yes)
                {
                    Control c = HUD_View.Controls[HUD_View.Controls.IndexOfKey((string)updateButton.Tag)];
                    if (textEditPanel.Visible)
                    {
                        if (textChange.Text != "") c.Text = textChange.Text;
                        else MessageBox.Show("Text element cannot be empty", "Update Element", MessageBoxButtons.OK);
                        c.ForeColor = colorPicker.BackColor;
                        c.Font = new Font(c.Font.Name, (float)sizeChange.Value, GraphicsUnit.Pixel);
                    }
                    else // Updating image
                    {

                    }

                    if (nameChange.Text != "" && !HUD_View.Controls.ContainsKey(nameChange.Text)) c.Name = nameChange.Text;
                    else if (nameChange.Text != c.Name) MessageBox.Show("Please enter a name for the element that does not match the name of a different element.", "Update Element", MessageBoxButtons.OK);
                    editLabel.Text = "Edit [" + c.Name + "]:";
                }
            };
        }

        private void CreateElement_Click(object sender, EventArgs e)
        {
            if (nameBox.Text != "" && !HUD_View.Controls.ContainsKey(nameBox.Text))
            {
                if ((string)typeSelector.SelectedItem == "Text")
                {
                    Label newElement = new Label
                    {
                        AutoSize = true,
                        Name = nameBox.Text,
                        Text = "Text",
                        Cursor = Cursors.SizeAll,
                        Enabled = true,
                        AllowDrop = false // Use this to do drag dropping
                    };
                    newElement.Font = new Font(newElement.Font.Name, 10, GraphicsUnit.Pixel);
                    HUD_View.Controls.Add(newElement);
                    newElement.Location = new Point(640, 360);

                    // Allow editing on click
                    newElement.Click += delegate
                    {
                        editLabel.Text = "Edit [" + newElement.Name + "]:";
                        nameChange.Text = newElement.Name;
                        textChange.Text = newElement.Text;
                        colorPicker.BackColor = newElement.ForeColor;
                        colorDialog.Color = newElement.ForeColor;
                        updateButton.Tag = newElement.Name;
                        sizeChange.Value = (decimal)newElement.Font.Size;
                    };

                    newElement.MouseDown += delegate
                    {
                        newElement.AllowDrop = true;
                    };

                    newElement.MouseMove += delegate
                    {
                        if (newElement.AllowDrop)
                        {
                            Point p = PointToClient(new Point(Cursor.Position.X - 5, Cursor.Position.Y - 48));
                            int l = 0;
                            int r = HUD_View.Size.Width;
                            int t = 0;
                            int b = HUD_View.Size.Height;
                            if (p.X + newElement.Width > r)
                            {
                                p.X = r - newElement.Width;
                            }
                            if (p.X < l)
                            {
                                p.X = l;
                            }
                            if (p.Y + newElement.Height > b)
                            {
                                p.Y = b - newElement.Height;
                            }
                            if (p.Y < t)
                            {
                                p.Y = t;
                            }
                            newElement.Location = p;

                        }
                    };

                    newElement.MouseUp += delegate
                    {
                    //newElement.DoDragDrop(newElement, DragDropEffects.Move);
                    newElement.AllowDrop = false;
                    };
                }
                else
                {
                    MessageBox.Show("Please select a valid element type", "Create Element", MessageBoxButtons.OK);
                }
            }
            else
            {
                MessageBox.Show("Please enter a name for the element that does not match the name of a different element.", "Create Element", MessageBoxButtons.OK);
            }
        }

        private void ExitButton_Click(object sender, EventArgs e)
        {
            DialogResult dialog = MessageBox.Show("Are you sure you want to exit?", "Exit Editor", MessageBoxButtons.YesNo);
            if (dialog == DialogResult.Yes) Close();
        }

        private void ExportButton_Click(object sender, EventArgs e)
        {
            FileStream fstream;
            try
            {
                fstream = new FileStream("..\\..\\..\\Assets\\GameHUD.json", FileMode.CreateNew, FileAccess.ReadWrite);
            }
            catch (IOException)
            {
                DialogResult dialog = MessageBox.Show("A HUD file already exists, if you continue it will be overwritten.", "Overwrite File", MessageBoxButtons.OKCancel);
                if (dialog == DialogResult.OK)
                {
                    fstream = new FileStream("..\\..\\..\\Assets\\GameHUD.json", FileMode.Create, FileAccess.ReadWrite);
                }
                else return;
            }
            StreamWriter fwriter = new StreamWriter(fstream);
            fwriter.WriteLine("{");
            for (int i = 0; i < HUD_View.Controls.Count; i++) // Save the json data
            {
                Control c = HUD_View.Controls[i];
                fwriter.WriteLine("  \""+c.Name+"\": {");
                if ((string)c.Tag == "Image") // YGWIOL>UEGFHUIWQGVBFHVOQLWHUYVGFYUHVOWEIUHJFGUIWQGYUIHEF YOU MUST TAG IMAGES AS IMAGE
                {
                    fwriter.WriteLine("    \"type\": \"image\",");

                }
                else
                {
                    fwriter.WriteLine("    \"type\": \"text\",");
                    fwriter.WriteLine("    \"text\": \""+c.Text+"\",");
                    fwriter.WriteLine("    \"position\": \""+c.Location.X+","+c.Location.Y+"\",");
                    fwriter.WriteLine("    \"font\": \""+c.Font.Name+"\",");
                    fwriter.WriteLine("    \"size\": \""+c.Font.Size/40+"\",");
                    fwriter.WriteLine("    \"color\": \""+c.ForeColor.R+","+c.ForeColor.G+","+c.ForeColor.B+","+c.ForeColor.A+"\"");
                    if (i < HUD_View.Controls.Count-1) fwriter.WriteLine("  },");
                    else fwriter.WriteLine("  }");
                }
            }
            fwriter.WriteLine("}");

            fwriter.Close();
            fstream.Close();
            MessageBox.Show("HUD File successfully saved.", "Save File", MessageBoxButtons.OK);
        }
    }
}
