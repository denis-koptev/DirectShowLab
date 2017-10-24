#pragma once

#include <dshow.h>
#include "DShowVideo.h"

namespace dshow_lab3
{

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

/// <summary>
/// Summary for VideoForm
/// </summary>
public ref class VideoForm : public System::Windows::Forms::Form
{
    public:

        void * hWnd = 0;
        DShowVideo * dShowVideo;
        bool fromCamera = false;

    private:

        System::Windows::Forms::CheckBox ^ sourceBox;
        System::Windows::Forms::Button ^ stopButton;

    public:

        VideoForm()
        {
            InitializeComponent();
            //
            //TODO: Add the constructor code here
            //

            // GET WINDOW ID
            //hWnd = Handle.ToPointer();
            hWnd = videoPanel->Handle.ToPointer();
        }

    protected:
        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        ~VideoForm()
        {
            if (dShowVideo != NULL)
            {
                dShowVideo->deinitialize();
            }

            if (components)
            {
                delete components;
            }

        }

    private:

        System::Windows::Forms::Panel ^ videoPanel;
        System::Windows::Forms::Button ^ startButton;
        System::Windows::Forms::Button ^ pauseButton;

        /// <summary>
        /// Required designer variable.
        /// </summary>
        System::ComponentModel::Container ^ components;

        #pragma region Windows Form Designer generated code
        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        void InitializeComponent(void)
        {
            this->videoPanel = (gcnew System::Windows::Forms::Panel());
            this->startButton = (gcnew System::Windows::Forms::Button());
            this->pauseButton = (gcnew System::Windows::Forms::Button());
            this->stopButton = (gcnew System::Windows::Forms::Button());
            this->sourceBox = (gcnew System::Windows::Forms::CheckBox());
            this->SuspendLayout();
            //
            // videoPanel
            //
            this->videoPanel->BackColor = System::Drawing::SystemColors::ActiveBorder;
            this->videoPanel->Location = System::Drawing::Point(200, 0);
            this->videoPanel->Name = L"videoPanel";
            this->videoPanel->Size = System::Drawing::Size(400, 400);
            this->videoPanel->TabIndex = 0;
            //
            // startButton
            //
            this->startButton->Location = System::Drawing::Point(255, 426);
            this->startButton->Name = L"startButton";
            this->startButton->Size = System::Drawing::Size(70, 23);
            this->startButton->TabIndex = 1;
            this->startButton->Text = L"Start";
            this->startButton->UseVisualStyleBackColor = true;
            this->startButton->Click += gcnew System::EventHandler(this, &VideoForm::startButton_Click);
            //
            // pauseButton
            //
            this->pauseButton->Location = System::Drawing::Point(365, 426);
            this->pauseButton->Name = L"pauseButton";
            this->pauseButton->Size = System::Drawing::Size(70, 23);
            this->pauseButton->TabIndex = 1;
            this->pauseButton->Text = L"Pause";
            this->pauseButton->UseVisualStyleBackColor = true;
            this->pauseButton->Click += gcnew System::EventHandler(this, &VideoForm::pauseButton_Click);
            //
            // stopButton
            //
            this->stopButton->Location = System::Drawing::Point(475, 425);
            this->stopButton->Name = L"stopButton";
            this->stopButton->Size = System::Drawing::Size(70, 23);
            this->stopButton->TabIndex = 2;
            this->stopButton->Text = L"Stop";
            this->stopButton->UseVisualStyleBackColor = true;
            this->stopButton->Click += gcnew System::EventHandler(this, &VideoForm::stopButton_Click);
            //
            // sourceBox
            //
            this->sourceBox->AutoSize = true;
            this->sourceBox->Location = System::Drawing::Point(12, 12);
            this->sourceBox->Name = L"sourceBox";
            this->sourceBox->Size = System::Drawing::Size(125, 17);
            this->sourceBox->TabIndex = 3;
            this->sourceBox->Text = L"Capture from Camera";
            this->sourceBox->UseVisualStyleBackColor = true;
            this->sourceBox->CheckedChanged += gcnew System::EventHandler(this, &VideoForm::sourceBox_CheckedChanged);
            //
            // VideoForm
            //
            this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
            this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
            this->ClientSize = System::Drawing::Size(784, 461);
            this->Controls->Add(this->sourceBox);
            this->Controls->Add(this->stopButton);
            this->Controls->Add(this->pauseButton);
            this->Controls->Add(this->startButton);
            this->Controls->Add(this->videoPanel);
            this->Name = L"VideoForm";
            this->Text = L"VideoForm";
            this->ResumeLayout(false);
            this->PerformLayout();

        }
        #pragma endregion

        System::Void startButton_Click(System::Object ^ sender, System::EventArgs ^ e)
        {
            if (dShowVideo == NULL)
            {
                dShowVideo = new DShowVideo();
            }

            dShowVideo->setSource(sourceBox->Checked);
            int code = dShowVideo->initialize(L"clock.avi", hWnd=0);

            if (code)
            {
                String ^ errString = gcnew String(dShowVideo->getErrorMessage(code));
                MessageBox::Show(errString);
                return;
            }

            code = dShowVideo->startVideo();

            if (code)
            {
                String ^ errString = gcnew String(dShowVideo->getErrorMessage(code));
                MessageBox::Show(errString);
            }
        }

        System::Void pauseButton_Click(System::Object ^ sender, System::EventArgs ^ e)
        {
            if (dShowVideo != NULL)
            {
                int code = dShowVideo->pauseVideo();

                if (code)
                {
                    String ^ errString = gcnew String(dShowVideo->getErrorMessage(code));
                    MessageBox::Show(errString);
                }
            }
        }

        System::Void stopButton_Click(System::Object ^ sender, System::EventArgs ^ e)
        {
            if (dShowVideo != NULL)
            {
                dShowVideo->pauseVideo();
                dShowVideo->deinitialize();
                delete dShowVideo;
                dShowVideo = NULL;
            }
        }

        System::Void sourceBox_CheckedChanged(System::Object ^ sender, System::EventArgs ^ e) {
        }

};
}
