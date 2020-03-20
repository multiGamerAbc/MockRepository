///////////////////////////////////////////////////////////////////////
// MainWindow.xaml.cs - GUI for Project3HelpWPF                      //
// ver 1.0                                                           //
// Author: Phil Maalouf, pmaalouf@syr.edu                            //
// Source: Jim Fawcett, CSE687 - Object Oriented Design, Spring 2018 //
///////////////////////////////////////////////////////////////////////
/*
 * Package Operations:
 * -------------------
 * This package provides a WPF-based GUI for Project3HelpWPF demo.  It's 
 * responsibilities are to:
 * - Provide a display of directory contents of a remote ServerPrototype.
 * - It provides a subdirectory list and a filelist for the selected directory.
 * - You can navigate into subdirectories by double-clicking on subdirectory
 *   or the parent directory, indicated by the name "..".
 *   
 * Required Files:
 * ---------------
 * Mainwindow.xaml, MainWindow.xaml.cs
 * Translater.dll
 * 
 * Maintenance History:
 * --------------------
 * ver 1.0 : 30 Mar 2018
 * - first release
 * - Several early prototypes were discussed in class. Those are all superceded
 *   by this package.
 *   
 * - Modified by Phil Maalouf 27 June 2018
 */

// Translater has to be statically linked with CommLibWrapper
// - loader can't find Translater.dll dependent CommLibWrapper.dll

//Cite: https://stackoverflow.com/questions/7210158/linebreak-in-textbox
//Cite: https://www.c-sharpcorner.com/uploadfile/mahesh/listbox-in-wpf/  List box add/remove


using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.Threading;
using System.IO;
using MsgPassingCommunication;
namespace WpfApp1
{
    public partial class MainWindow : Window
    {
        //----< Main Window Initialization >------------------------------------

        public MainWindow()
        {
            InitializeComponent();
            Console.Title = "Mock Repository GUI Console";
        }

        //----< variables >-----------------------------------------------------
        private Stack<string> pathStack_ = new Stack<string>();
        private Translater translater;
        private Translater translater2;
        private CsEndPoint endPoint_;
        private CsEndPoint endPoint2_;
        private Thread rcvThrd = null;
        private Dictionary<string, Action<CsMessage>> dispatcher_
          = new Dictionary<string, Action<CsMessage>>();
        private string currentPath = "";
        private string currentPathForRepoListBox = "../MockRepo";
        private string currentPathForUserListBox = "../UserMachineDir";
        private bool isLoggedIn = false;
        private int testNum = 1;

        //----< search repo for a given file >----------------------------------

        private void SearchForFile_Click(object sender, RoutedEventArgs e){
            CsEndPoint userEndPoint = new CsEndPoint();
            userEndPoint.machineAddress = "localhost";
            userEndPoint.port = 9000;
            CsMessage msg = new CsMessage();
            msg.add("to", CsEndPoint.toString(userEndPoint));
            msg.add("from", CsEndPoint.toString(endPoint_));
            msg.add("command", "Search_For_File");
            msg.add("searchFile", SearchInputBox.Text);
            translater.postMessage(msg);
            CsMessage replyMsg = translater.getMessage();
            SearchInputBox.Text = replyMsg.value("fileSearchResult");
        }

        //----< search repo for a given directory >-----------------------------

        private void SearchForDir_Click(object sender, RoutedEventArgs e)
        {
            CsEndPoint userEndPoint = new CsEndPoint();
            userEndPoint.machineAddress = "localhost";
            userEndPoint.port = 9000;
            CsMessage msg = new CsMessage();
            msg.add("to", CsEndPoint.toString(userEndPoint));
            msg.add("from", CsEndPoint.toString(endPoint_));
            msg.add("command", "Search_For_Dir");
            msg.add("searchDirName", SearchInputBox.Text);
            translater.postMessage(msg);
            CsMessage replyMsg = translater.getMessage();
            SearchInputBox.Text = replyMsg.value("dirSearchResult");
        }

        //----< upload selected file to current repo directory >----------------

        private void UploadButton_Click(object sender, RoutedEventArgs e)
        {
            if (UploadTextBox.Text.Length <= 0)
            {
                UploadTextBox.Text = "Must Choose File To Download";
            }
            else
            {
                CsEndPoint userEndPoint = new CsEndPoint();
                userEndPoint.machineAddress = "localhost";
                userEndPoint.port = 9000;
                CsMessage msg = new CsMessage();
                msg.add("to", CsEndPoint.toString(userEndPoint));
                msg.add("from", CsEndPoint.toString(endPoint_));
                msg.add("command", "Upload_File");
                msg.add("fileName", UploadTextBox.Text);
                msg.add("fromPath", currentPathForUserListBox);
                msg.add("toPath", currentPathForRepoListBox);
                translater.postMessage(msg);
            }
        }

        //----< download selected file to current user directory >--------------

        private void DownloadButton_Click(object sender, RoutedEventArgs e)
        {
            if (DownloadTextBox.Text.Length <= 0)
            {
                DownloadTextBox.Text = "Must Choose File To Download";
            }
            else
            {
                CsEndPoint userEndPoint = new CsEndPoint();
                userEndPoint.machineAddress = "localhost";
                userEndPoint.port = 9000;
                CsMessage msg = new CsMessage();
                msg.add("to", CsEndPoint.toString(userEndPoint));
                msg.add("from", CsEndPoint.toString(endPoint_));
                msg.add("command", "Download_File");
                msg.add("fileName", DownloadTextBox.Text);
                msg.add("fromPath", currentPathForRepoListBox);
                msg.add("toPath", currentPathForUserListBox);
                translater.postMessage(msg);
            }
        }

        //----< navigate repo or set contents of DownloadTextBox >--------------

        private void RepoListBox_MouseDoubleClick(object sender, RoutedEventArgs e)
        {
            if (RepoListBox.SelectedItem.ToString().Substring(0, 5) == "<dir>" ||
                RepoListBox.SelectedItem.ToString() == "<-- Back" ||
                RepoListBox.SelectedItem.ToString() == "MockRepo"
                )
            {
                navigateForRepoListBox();
            }
            else
            {
                DownloadTextBox.Text = RepoListBox.SelectedItem.ToString();
            }
        }

        //----< navigate user machine or set contents of uploadTextBox >--------

        private void UserListBox_MouseDoubleClick(object sender, RoutedEventArgs e)
        {
            if (UserListBox.SelectedItem.ToString().Substring(0, 5) == "<dir>" ||
                UserListBox.SelectedItem.ToString() == "<-- Back" ||
                UserListBox.SelectedItem.ToString() == "UserMachineDir"
                )
            {
                navigateForUserListBox();
            }
            else
            {
                UploadTextBox.Text = UserListBox.SelectedItem.ToString();
            }
        }

        //----<  Window_Loaded  >-----------------------------------------------

        private void Window_Loaded(object sender, RoutedEventArgs e)
        {
            // start Comm
            endPoint_ = new CsEndPoint();
            endPoint_.machineAddress = "localhost";
            endPoint_.port = 9001;
            translater = new Translater();
            translater.listen(endPoint_);

            endPoint2_ = new CsEndPoint();
            endPoint2_.machineAddress = "localhost";
            endPoint2_.port = 9002;
            translater2 = new Translater();
            translater2.listen(endPoint2_);

            CsEndPoint userEndPoint = new CsEndPoint();
            userEndPoint.machineAddress = "localhost";
            userEndPoint.port = 9000;

            CsMessage msg = new CsMessage();
            msg.add("to", CsEndPoint.toString(userEndPoint));
            msg.add("from", CsEndPoint.toString(endPoint_));
            msg.add("command", "Hello from GUI");
            translater.postMessage(msg);

            tabItem2.IsEnabled = false;
            tabItem3.IsEnabled = false;
            tabItem4.IsEnabled = false;

            // For development - so I don't have to log in manually
            // Comment for normal use
            //tabItem2.IsEnabled = true;
            //tabItem3.IsEnabled = true;
            //tabItem4.IsEnabled = true;

            LeftListBox.Items.Add("MockRepo");
            RepoListBox.Items.Add("MockRepo");
            UserListBox.Items.Add("UserMachineDir");
        }

        //----< navigation logic for user machine directory >-------------------

        private void navigateForUserListBox()
        {
            CsEndPoint userEndPoint = new CsEndPoint();
            userEndPoint.machineAddress = "localhost";
            userEndPoint.port = 9000;
            CsMessage msg = new CsMessage();
            msg.add("to", CsEndPoint.toString(userEndPoint));
            msg.add("from", CsEndPoint.toString(endPoint_));
            msg.add("command", "Show_Contents");
            if (currentPathForUserListBox.Length == 0)
            {
                currentPathForUserListBox = "../UserMachineDir";
            }
            else if (UserListBox.SelectedItem.ToString().Substring(0, 5) == "<dir>")
            {
                currentPathForUserListBox += "/" + UserListBox.SelectedItem.ToString().Substring(6);
            }
            else if (UserListBox.SelectedItem.ToString() == "<-- Back")
            {
                int lastIndex = currentPathForUserListBox.LastIndexOf('/');
                currentPathForUserListBox = currentPathForUserListBox.Substring(0, lastIndex);
            }
            if (currentPathForUserListBox == "..")
            {
                currentPathForUserListBox = "../UserMachineDir";
                UserListBox.Items.Clear();
                UserListBox.Items.Add("UserMachineDir");
                return;
            }
            msg.add("dir_name", currentPathForUserListBox);
            translater.postMessage(msg);
            CsMessage replyMsg = translater.getMessage();
            replyMsg.show();
            string contentsString = replyMsg.value("Contents_String");
            string[] contentsArray = contentsString.Split('$');
            UserListBox.Items.Clear();
            UserListBox.Items.Add("<-- Back");
            for (int i = 0; i < contentsArray.Length; i++)
            {
                if (contentsArray[i] != "." && contentsArray[i] != "..")
                    UserListBox.Items.Add(contentsArray[i]);
            }
        }

        //----< navigation logic for repo >-------------------------------------

        private void navigateForRepoListBox()
        {
            CsEndPoint userEndPoint = new CsEndPoint();
            userEndPoint.machineAddress = "localhost";
            userEndPoint.port = 9000;
            CsMessage msg = new CsMessage();
            msg.add("to", CsEndPoint.toString(userEndPoint));
            msg.add("from", CsEndPoint.toString(endPoint_));
            msg.add("command", "Show_Contents");
            if (currentPathForRepoListBox.Length == 0)
            {
                currentPathForRepoListBox = "../MockRepo";
            }
            else if (RepoListBox.SelectedItem.ToString().Substring(0, 5) == "<dir>")
            {
                currentPathForRepoListBox += "/" + RepoListBox.SelectedItem.ToString().Substring(6);
            }
            else if (RepoListBox.SelectedItem.ToString() == "<-- Back")
            {
                int lastIndex = currentPathForRepoListBox.LastIndexOf('/');
                currentPathForRepoListBox = currentPathForRepoListBox.Substring(0, lastIndex);
            }
            if (currentPathForRepoListBox == "..")
            {
                currentPathForRepoListBox = "../MockRepo";
                RepoListBox.Items.Clear();
                RepoListBox.Items.Add("MockRepo");
                return;
            }      
            msg.add("dir_name", currentPathForRepoListBox);
            translater.postMessage(msg);
            CsMessage replyMsg = translater.getMessage();
            replyMsg.show();
            string contentsString = replyMsg.value("Contents_String");
            string[] contentsArray = contentsString.Split('$');
            RepoListBox.Items.Clear();
            RepoListBox.Items.Add("<-- Back");
            for (int i = 0; i < contentsArray.Length; i++)
            {
                if (contentsArray[i] != "." && contentsArray[i] != "..")
                    RepoListBox.Items.Add(contentsArray[i]);
            }
        }

        //----< navigation logic for repo (used in Build/Submit page) >---------

        private void navigate()
        {
            CsEndPoint userEndPoint = new CsEndPoint();
            userEndPoint.machineAddress = "localhost";
            userEndPoint.port = 9000;
            CsMessage msg = new CsMessage();
            msg.add("to", CsEndPoint.toString(userEndPoint));
            msg.add("from", CsEndPoint.toString(endPoint_));
            msg.add("command", "Show_Contents");
            if (currentPath.Length == 0)
            {
                currentPath = "../MockRepo";
            }
            else if (LeftListBox.SelectedItem.ToString().Substring(0, 5) == "<dir>")
            {
                currentPath += "/" + LeftListBox.SelectedItem.ToString().Substring(6);
            }
            else if (LeftListBox.SelectedItem.ToString() == "<-- Back")
            {
                int lastIndex = currentPath.LastIndexOf('/');
                currentPath = currentPath.Substring(0, lastIndex);
            }
            if (currentPath == "..")
            {
                currentPath = "../MockRepo";
                LeftListBox.Items.Clear();
                LeftListBox.Items.Add("MockRepo");
                return;
            }
            msg.add("dir_name", currentPath);
            translater.postMessage(msg);
            CsMessage replyMsg = translater.getMessage();
            // No point in checking command with this esoteric (and brittle) 
            // message passing structure, but it should be "Show_Contents_Reply".
            replyMsg.show();
            string contentsString = replyMsg.value("Contents_String");
            string[] contentsArray = contentsString.Split('$');
            LeftListBox.Items.Clear();
            LeftListBox.Items.Add("<-- Back");
            for (int i = 0; i < contentsArray.Length; i++)
            {
                if (contentsArray[i] != "." && contentsArray[i] != "..")
                    LeftListBox.Items.Add(contentsArray[i]);
            }
        }

        //----< submit test request >-------------------------------------------

        private void SubmitTestRequestButton_Click(object sender, RoutedEventArgs e)
        {
            CsEndPoint userEndPoint = new CsEndPoint();
            userEndPoint.machineAddress = "localhost";
            userEndPoint.port = 9000;
            CsMessage msg = new CsMessage();
            msg.add("to", CsEndPoint.toString(userEndPoint));
            msg.add("from", CsEndPoint.toString(endPoint_));
            string testDriverElement = TestDriverListBox.Items[0].ToString();
            int arraySize = TestedCodeListBox.Items.Count;
            string[] testedCodeElements = TestedCodeListBox.Items.OfType<string>().ToArray();
            msg.add("command", "Test_Request");
            string str = CreateTestRequestString(testDriverElement, testedCodeElements);
            string testNumString = testNum.ToString();
           // msg.add("testNumString", testNumString);
            msg.add("name", str);
            translater.postMessage(msg);
            string forStatusListBox = CreateTextBoxStringFromTestRequestInfo(testDriverElement, testedCodeElements);
            string statusMsg = forStatusListBox + "Awaiting Status Msg";
            //"\nTest Number " + testNum.ToString() + ":\n" + 
            updateStatusListBox(statusMsg);
            CsMessage msgIn = translater2.getMessage();
            if (msgIn.value("command") == "Test Status Message")
            {
                msgIn.show();
                string statusReplyMsg =
                    "Test Number:     " + msgIn.value("testRequestID") +
                    "\nStatus:          " + msgIn.value("status") +
                    "\nTime Date Stamp: " + msgIn.value("TimeDateStamp") +
                    "\nTest Log File:   " + msgIn.value("TestLogFileString") + "\n";
                StatusListBox.Items.Add(statusReplyMsg);
            }
            testNum++;
        }

        //----< Create Test Request String >------------------------------------

        private string CreateTestRequestString(string testDriverElement, string[] testedCodeElements)
        {
            string result = "";
            result += "<TestElement><TestDriverElem>" + testDriverElement.Substring(0, testDriverElement.Length - 4);
            result += "</TestDriverElem>";
            for (int i = 0; i < testedCodeElements.Length; i++)
            {
                result += "<TestedCodeElem>" +
                    testedCodeElements[i].Substring(0, testedCodeElements[i].Length - 4) +
                    "</TestedCodeElem>";
            }
            result += "/TestElement>";
            return result;
        }

        //----< Create TextBox String From Test Request Info >------------------

        private string CreateTextBoxStringFromTestRequestInfo(string testDriverElement, string[] testedCodeElements)
        {
            string result = "";
            result += testDriverElement + "\n";
            for (int i = 0; i < testedCodeElements.Length; i++)
            {
                result += testedCodeElements[i] + "\n";
            }
            return result;
        }

        //----< Add Test Status Message >---------------------------------------

        private void updateStatusListBox(string str)
        {
            StatusListBox.Items.Add(str);
        }

        //----< Handle New User Button Click >----------------------------------

        private void CreateNewUserButton_Click(object sender, RoutedEventArgs e)
        {
            if(CreateNewUserButton.Content == "Create New User")
            {
                CreateNewUserButton.Content = "Submit";
                NewUserInputPanel.Visibility = Visibility.Visible;
            } else
            {
                CsEndPoint userEndPoint = new CsEndPoint();
                userEndPoint.machineAddress = "localhost";
                userEndPoint.port = 9000;
                CsMessage msg = new CsMessage();
                msg.add("to", CsEndPoint.toString(userEndPoint));
                msg.add("from", CsEndPoint.toString(endPoint_));
                msg.add("command", "create_new_user");
                msg.add("userName", NewUserNameTextBox.Text);
                msg.add("password", NewUserPasswordTextBox.Text);
                translater.postMessage(msg);
                CreateNewUserButton.Content = "Create New User";
                NewUserInputPanel.Visibility = Visibility.Hidden;
            }
        }

        //----< Handle Login Button Click >-------------------------------------

        private void LogInButton_Click(object sender, RoutedEventArgs e)
        {
            if (!isLoggedIn)
            {
                CsEndPoint userEndPoint = new CsEndPoint();
                userEndPoint.machineAddress = "localhost";
                userEndPoint.port = 9000;
                CsMessage msg = new CsMessage();
                msg.add("to", CsEndPoint.toString(userEndPoint));
                msg.add("from", CsEndPoint.toString(endPoint_));
                msg.add("command", "check_credentials");
                msg.add("userName", UserNameBox.Text);
                msg.add("password", PasswordBox.Text);
                UserNameBox.Clear();
                PasswordBox.Clear();
                translater.postMessage(msg);
                CsMessage replyMsg = translater.getMessage();
                if (replyMsg.value("credentialCheck") == "credential_good_is_admin")
                {
                    UserNameBox.Clear();
                    PasswordBox.Clear();
                    isLoggedIn = true;
                    LogInButton.Content = "Log Out";
                    CreateNewUserButton.IsEnabled = true;
                    tabItem2.IsEnabled = true;
                    tabItem3.IsEnabled = true;
                    tabItem4.IsEnabled = true;
                    Console.Write("\nValid - Admin");
                }
                else if (replyMsg.value("credentialCheck") == "credential_good_not_admin")
                {
                    UserNameBox.Clear();
                    PasswordBox.Clear();
                    isLoggedIn = true;
                    LogInButton.Content = "Log Out";
                    tabItem2.IsEnabled = true;
                    tabItem3.IsEnabled = true;
                    tabItem4.IsEnabled = true;
                    Console.Write("\nValid - Not Admin");
                }
                else if (replyMsg.value("credentialCheck") == "credential_not_good")
                {
                    Console.Write("\nNot Valid");
                }
                else
                {
                    Console.Write("\nLog In Error");
                }
            } else
            {
                UserNameBox.Clear();
                PasswordBox.Clear();
                isLoggedIn = false;
                LogInButton.Content = "Log In";
                CreateNewUserButton.IsEnabled = false;
                CreateNewUserButton.Content = "Create New User";
                tabItem2.IsEnabled = false;
                tabItem3.IsEnabled = false;
                tabItem4.IsEnabled = false;
                Console.Write("\nLogging Out");
            }

        }

        //----< Stop Server Gracefully >----------------------------------------
        private void StopServerButton_Click(object sender, RoutedEventArgs e)
        {
            CsEndPoint userEndPoint = new CsEndPoint();
            userEndPoint.machineAddress = "localhost";
            userEndPoint.port = 9000;
            CsMessage msg = new CsMessage();
            msg.add("to", CsEndPoint.toString(userEndPoint));
            msg.add("from", CsEndPoint.toString(endPoint_));
            msg.add("command", "stop");
            translater.postMessage(msg);
        }

        //--< either navigate or add test driver or tested code element to test request >--

        private void ListBox_MouseDoubleClick(object sender, RoutedEventArgs e)
        {
            if (LeftListBox.SelectedItem.ToString().Substring(0, 5) == "<dir>" ||
                LeftListBox.SelectedItem.ToString() == "<-- Back"              ||
                LeftListBox.SelectedItem.ToString() == "MockRepo"
                )
            {
                navigate();
            } else
            {
                if (LeftListBox.SelectedItem.ToString().Contains("Driver"))
                {
                    TestDriverListBox.Items.Clear();
                    TestDriverListBox.Items.Add(LeftListBox.SelectedItem.ToString());
                }
                else
                {
                    TestedCodeListBox.Items.Add(LeftListBox.SelectedItem.ToString());
                }
            }
        }

        //----< Clear Test Element from test request being built >--------------

        private void ClearTestElementButton_Click(object sender, RoutedEventArgs e)
        {
            TestDriverListBox.Items.Clear();
        }

        //----< Clear Tested Code Elements from test request being built >------

        private void ClearTestedCodeElementsButton_Click(object sender, RoutedEventArgs e)
        {
            TestedCodeListBox.Items.Clear();
        }

        //----< tell server to create a directory >-----------------------------

        private void MakeDirButton_Click(object sender, RoutedEventArgs e)
        {
            CsEndPoint userEndPoint = new CsEndPoint();
            userEndPoint.machineAddress = "localhost";
            userEndPoint.port = 9000;
            CsMessage msg = new CsMessage();
            msg.add("to", CsEndPoint.toString(userEndPoint));
            msg.add("from", CsEndPoint.toString(endPoint_));
            msg.add("command", "Make_Dir");
            msg.add("dir_name", MakeDirInput.Text);
            translater.postMessage(msg);
        }
    }
}
//------------------------------------------------------------------------------
        //----------------------------------------------------------------------
//////////////////////////////////////////////////////////////////////////////

//============================================================================

//////////////////////////////////////////////////////////////////////////////

//============================================================================