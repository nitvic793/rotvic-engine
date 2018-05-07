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

            changeTint.Click += delegate
            {
                if (colorDialog.ShowDialog() == DialogResult.OK)
                {
                    changeTint.BackColor = colorDialog.Color;
                }
            };
            //string combinedPath = Path.GetFullPath(Path.Combine(Directory.GetCurrentDirectory(), "..\\..\\..\\Assets\\Images")); // From https://stackoverflow.com/questions/21769921/does-openfiledialog-initialdirectory-not-accept-relative-path
            //openFileDialog.InitialDirectory = combinedPath;

            string combinedPath = Path.GetFullPath(Path.Combine(Directory.GetCurrentDirectory(), "..\\..\\..\\Assets\\Images")); // From https://stackoverflow.com/questions/21769921/does-openfiledialog-initialdirectory-not-accept-relative-path
            openFileDialog.InitialDirectory = combinedPath;
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
                        Text = creationBox.Text == "" ? "Text" : creationBox.Text,
                        Cursor = Cursors.SizeAll,
                        Enabled = true,
                        Tag = "Text",
                        BackColor = Color.Transparent,
                        AllowDrop = false // Use this to do drag dropping
                    };
                    newElement.Font = new Font(newElement.Font.Name, 40, GraphicsUnit.Pixel);
                    HUD_View.Controls.Add(newElement);
                    newElement.Location = new Point(640, 360);

                    // Allow editing on click
                    newElement.Click += delegate
                    {
                        imageEditPanel.Hide();
                        textEditPanel.BringToFront();
                        textEditPanel.Show();
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
                else if ((string)typeSelector.SelectedItem == "Image")
                {
                    if (creationBox.Text != "")
                    {
                        PictureBox newElement;
                        try
                        {
                            newElement = new PictureBox
                            {
                                Name = nameBox.Text,
                                SizeMode = PictureBoxSizeMode.AutoSize,
                                ImageLocation = creationBox.Text,
                                Cursor = Cursors.SizeAll,
                                Enabled = true,
                                BackColor = Color.Transparent,
                                AllowDrop = false // Use this to do drag dropping
                            };
                        }
                        catch (Exception ex)
                        {
                            MessageBox.Show("Image element creation aborted, file not read correctly.\nFull error: " + ex.Message, "File Read Error", MessageBoxButtons.OK);
                            return;
                        }

                        HUD_View.Controls.Add(newElement);
                        newElement.Location = new Point(640, 360);

                        // Allow editing on click
                        newElement.Click += delegate
                        {
                            if (newElement.SizeMode != PictureBoxSizeMode.StretchImage)
                            {
                                newElement.SizeMode = PictureBoxSizeMode.StretchImage;
                                newElement.Width = newElement.Image.Size.Width;
                                newElement.Height = newElement.Image.Size.Height;
                            }

                            textEditPanel.Hide();
                            imageEditPanel.BringToFront();
                            imageEditPanel.Show();
                            editLabel.Text = "Edit [" + newElement.Name + "]:";
                            nameChange.Text = newElement.Name;
                            updateButton.Tag = newElement.Name;
                            changeImage.Text = newElement.ImageLocation;
                            changeTint.BackColor = newElement.ForeColor;
                            widthBox.Value = (decimal)(newElement.Width) / (newElement.Image.Size.Width);
                            heightBox.Value = (decimal)(newElement.Height) / (newElement.Image.Size.Height);
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
                        MessageBox.Show("Please enter a file path to a valid image type", "Create Element", MessageBoxButtons.OK);
                    }

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
                if ((string)c.Tag != "Text")
                {
                    fwriter.WriteLine("    \"type\": \"image\",");
                    fwriter.WriteLine("    \"scale\": \"" + (decimal)(c.Width) / (((PictureBox)c).Image.Size.Width) + "," + (decimal)(c.Height) / (((PictureBox)c).Image.Size.Height) + "\",");
                    string filename = ((PictureBox)c).ImageLocation.Split('\\')[((PictureBox)c).ImageLocation.Split('\\').Length-1];
                    //string engineFile = "..\\..\\..\\Assets\\Images\\" + filename;
                    //string file = ((PictureBox)c).ImageLocation;
                    //((PictureBox)c).ImageLocation = "";
                    //System.Threading.Thread.Sleep(1000);
                    //File.Copy(file, engineFile, true);
                    //System.Threading.Thread.Sleep(1000);
                    //((PictureBox)c).ImageLocation = file;
                    //System.Threading.Thread.Sleep(1000);
                    fwriter.WriteLine("    \"source\": \"..\\..\\Assets\\Images\\" + filename +"\",");
                    fwriter.WriteLine("    \"position\": \"" + c.Location.X + "," + c.Location.Y + "\",");
                    fwriter.WriteLine("    \"color\": \"" + c.ForeColor.R + "," + c.ForeColor.G + "," + c.ForeColor.B + "," + c.ForeColor.A + "\"");
                    if (i < HUD_View.Controls.Count - 1) fwriter.WriteLine("  },");
                    else fwriter.WriteLine("  }");
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
            MessageBox.Show("HUD File successfully saved, make sure to transfer any images not in Assets/Images to be there before trying to load this HUD configuration in game.", "Save File", MessageBoxButtons.OK);
        }

        private void DeleteButton_Click(object sender, EventArgs e)
        {
            if (MessageBox.Show("Are you sure you wish to DELETE this element? You cannot undo this.", "DELETE ELEMENT", MessageBoxButtons.YesNo) == DialogResult.Yes)
            {
                Control c = HUD_View.Controls[HUD_View.Controls.IndexOfKey((string)updateButton.Tag)];
                HUD_View.Controls.Remove(c);
                c.Dispose();
            }
        }

        private void UpdateButton_Click(object sender, EventArgs e)
        {
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
                    try
                    {
                        if (changeImage.Text != "" && changeImage.Text != ((PictureBox)c).ImageLocation) ((PictureBox)c).ImageLocation = changeImage.Text;
                    }
                    catch (Exception ex)
                    {
                        MessageBox.Show("Image source not changed, file not read correctly.\nFull error: " + ex.Message, "File Read Error", MessageBoxButtons.OK);
                        return;
                    }

                    c.ForeColor = changeTint.BackColor;
                    c.Width = (int)(((PictureBox)c).Image.Size.Width * widthBox.Value);
                    c.Height = (int)(((PictureBox)c).Image.Size.Height * heightBox.Value);

                }

                if (nameChange.Text != "" && !HUD_View.Controls.ContainsKey(nameChange.Text)) c.Name = nameChange.Text;
                else if (nameChange.Text != c.Name) MessageBox.Show("Please enter a name for the element that does not match the name of a different element.", "Update Element", MessageBoxButtons.OK);
                editLabel.Text = "Edit [" + c.Name + "]:";
            }
        }

        private void TypeSelected(object sender, EventArgs e)
        {
            if ((string)typeSelector.SelectedItem == "Text")
            {
                creationLabel.Text = "Element Text:";
                toolTip1.SetToolTip(creationLabel, "The text the element will display");
                toolTip1.SetToolTip(creationBox, "The text the element will display");

            }
            else
            {
                creationLabel.Text = "Image Filepath:";
                toolTip1.SetToolTip(creationLabel, "The filepath to the image the element will display");
                toolTip1.SetToolTip(creationBox, "The filepath to the image the element will display");
            }
            nameBox.Text = "";
            creationBox.Text = "";
        }

        private void CreationBox_Click(object sender, EventArgs e)
        {
            if ((string)typeSelector.SelectedItem == "Image")
            {
                if (openFileDialog.ShowDialog() == DialogResult.OK)
                {
                    creationBox.Text = openFileDialog.FileName;
                }
            }
        }

        private void ChangeImage_Click(object sender, EventArgs e)
        {
            if (openFileDialog.ShowDialog() == DialogResult.OK)
            {
                changeImage.Text = openFileDialog.FileName;
            }
        }

        private void HeightBox_ValueChanged(object sender, EventArgs e)
        {
            if (lockAspectBox.Checked)
            {
                widthBox.Value = heightBox.Value;
            }
        }
        private void WidthBox_ValueChanged(object sender, EventArgs e)
        {
            if (lockAspectBox.Checked)
            {
                heightBox.Value = widthBox.Value;
            }
        }
    }
}
