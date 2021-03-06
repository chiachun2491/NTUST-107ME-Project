#include "MyForm.h"
using namespace System;
using namespace System::Windows::Forms;

[STAThread]
int main(array<String^>^ argv)
{
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);
	FourierTransform::MyForm windowsForm;
	Application::Run(%windowsForm);
}

namespace FourierTransform
{
	System::Void MyForm::openFileDialog1_FileOk(System::Object^  sender, System::ComponentModel::CancelEventArgs^  e)
	{
		//將影像讀入並設儲存至DataManager的資料結構中
		Bitmap^ OriginalImage = gcnew Bitmap(openFileDialog1->FileName);
		pictureBox_SourceImage->Image = OriginalImage;
		int scale = 1;
		int M = OriginalImage->Height * scale;
		int N = OriginalImage->Width * scale;

		dataManager = new DataManager(M, N);
		for (int i = 0; i < OriginalImage->Height; i++)
		{
			for (int j = 0; j < OriginalImage->Width; j++)
			{
				Color srcColor = OriginalImage->GetPixel(j, i); // 擷取每個點的顏色
				int srcGrey = srcColor.R*0.299 + srcColor.G*0.587 + srcColor.B*0.144; // 彩色三通道轉成灰階
				dataManager->SetPixel(j, i, srcGrey);
			}
		}

		std::cout << "-Image has been loaded-" << std::endl;
	}
	System::Void MyForm::loadFileToolStripMenuItem_Click(System::Object ^ sender, System::EventArgs ^ e)
	{
		openFileDialog1->ShowDialog();
	}
	System::Void MyForm::setResultAsInputToolStripMenuItem_Click(System::Object ^ sender, System::EventArgs ^ e)
	{
		int w = dataManager->GetImageWidth();
		int h = dataManager->GetImageHeight();
		Bitmap^ sImage = gcnew Bitmap(w, h);

		//將當前輸出影像作為輸入影像
		for (int i = 0; i < w; i++)
		{
			for (int j = 0; j < h; j++)
			{
				int valuePixeli = dataManager->GetOutputImage()[i][j];

				if (valuePixeli > 255)
				{
					valuePixeli = 255;
				}
				else if (valuePixeli < 0)
				{
					valuePixeli = 0;
				}
				dataManager->SetPixel(j, i, valuePixeli);
				sImage->SetPixel(j, i, Color::FromArgb(valuePixeli, valuePixeli, valuePixeli));
			}
		}
		pictureBox_SourceImage->Image = sImage;
	}
	System::Void MyForm::fastFourierTransformToolStripMenuItem1_Click(System::Object ^ sender, System::EventArgs ^ e)
	{
		int w = dataManager->GetImageWidth();
		int h = dataManager->GetImageHeight();

		// 利用傅立葉之平移性，平移頻率
		for (int i = 0; i < h; i++)
		{
			for (int j = 0; j < w; j++)
			{
				int valuePixeli = dataManager->GetInputImage()[i][j];
				valuePixeli = valuePixeli * pow((float)-1, (float)(i + j));
				dataManager->SetPixel(j, i, valuePixeli);
			}
		}

		//將算出頻率資訊傳入輸出影像
		fourierTransformMethod->FastFourierTransform(dataManager->GetInputImage(), dataManager->GetOutputImage(), dataManager->GetFreqReal(), dataManager->GetFreqImag(), h, w);
		Bitmap^ FFTImage = gcnew Bitmap(w, h);
		for (int i = 0; i < h; i++)
		{
			for (int j = 0; j < w; j++)
			{
				int valuePixeli = dataManager->GetOutputImage()[i][j];
				if (valuePixeli > 255)
				{
					valuePixeli = 255;
				}
				else if (valuePixeli < 0)
				{
					valuePixeli = 0;
				}
				FFTImage->SetPixel(j, i, Color::FromArgb(valuePixeli, valuePixeli, valuePixeli));
			}
		}
		pictureBox_OutputImage->Image = FFTImage;
	}
	System::Void MyForm::inverseFastFourierTransformToolStripMenuItem1_Click(System::Object ^ sender, System::EventArgs ^ e)
	{
		int w = dataManager->GetImageWidth();
		int h = dataManager->GetImageHeight();

		// 利用傅立葉之平移性，平移頻率
		for (int i = 0; i < h; i++)
		{
			for (int j = 0; j < w; j++)
			{
				int valuePixeli = dataManager->GetInputImage()[i][j];
				valuePixeli = valuePixeli * pow((float)-1, (float)(i + j));
				dataManager->SetPixel(j, i, valuePixeli);
			}
		}

		//將算出頻率資訊傳入輸出影像
		fourierTransformMethod->InverseFastFourierTransform(dataManager->GetInputImage(), dataManager->GetOutputImage(), dataManager->GetFreqReal(), dataManager->GetFreqImag(), h, w);
		Bitmap^ InverseFFTImage = gcnew Bitmap(w, h);
		for (int i = 0; i < h; i++)
		{
			for (int j = 0; j < w; j++)
			{
				int valuePixeli = dataManager->GetOutputImage()[i][j];
				if (valuePixeli > 255)
				{
					valuePixeli = 255;
				}
				else if (valuePixeli < 0)
				{
					valuePixeli = 0;
				}
				InverseFFTImage->SetPixel(j, i, Color::FromArgb(valuePixeli, valuePixeli, valuePixeli));
			}
		}
		pictureBox_OutputImage->Image = InverseFFTImage;
	}
	System::Void MyForm::lowpassFilterToolStripMenuItem1_Click(System::Object ^ sender, System::EventArgs ^ e)
	{
		int w = dataManager->GetImageWidth();
		int h = dataManager->GetImageHeight();

		// 利用傅立葉之平移性，平移頻率
		for (int i = 0; i < h; i++)
		{
			for (int j = 0; j < w; j++)
			{
				int valuePixeli = dataManager->GetInputImage()[i][j];
				valuePixeli = valuePixeli * pow((float)-1, (float)(i + j));
				dataManager->SetPixel(j, i, valuePixeli);
			}
		}

		//將算出頻率資訊傳入輸出影像
		fourierTransformMethod->LowpassFilter(dataManager->GetFreqReal(), dataManager->GetFreqImag(), dataManager->GetOutputImage(), h, w);
		Bitmap^ LowpassImage = gcnew Bitmap(w, h);
		for (int i = 0; i < h; i++)
		{
			for (int j = 0; j < w; j++)
			{
				int valuePixeli = dataManager->GetOutputImage()[i][j];
				if (valuePixeli > 255)
				{
					valuePixeli = 255;
				}
				else if (valuePixeli < 0)
				{
					valuePixeli = 0;
				}
				LowpassImage->SetPixel(j, i, Color::FromArgb(valuePixeli, valuePixeli, valuePixeli));
			}
		}
		pictureBox_OutputImage->Image = LowpassImage;
	}
	System::Void MyForm::highpassFilterToolStripMenuItem1_Click(System::Object ^ sender, System::EventArgs ^ e)
	{
		int w = dataManager->GetImageWidth();
		int h = dataManager->GetImageHeight();

		// 利用傅立葉之平移性，平移頻率
		for (int i = 0; i < h; i++)
		{
			for (int j = 0; j < w; j++)
			{
				int valuePixeli = dataManager->GetInputImage()[i][j];
				valuePixeli = valuePixeli * pow((float)-1, (float)(i + j));
				dataManager->SetPixel(j, i, valuePixeli);
			}
		}

		//將算出頻率資訊傳入輸出影像
		fourierTransformMethod->HighpassFilter(dataManager->GetFreqReal(), dataManager->GetFreqImag(), dataManager->GetOutputImage(), h, w);
		Bitmap^ HighpassImage = gcnew Bitmap(w, h);
		for (int i = 0; i < h; i++)
		{
			for (int j = 0; j < w; j++)
			{
				int valuePixeli = dataManager->GetOutputImage()[i][j];
				if (valuePixeli > 255)
				{
					valuePixeli = 255;
				}
				else if (valuePixeli < 0)
				{
					valuePixeli = 0;
				}
				HighpassImage->SetPixel(j, i, Color::FromArgb(valuePixeli, valuePixeli, valuePixeli));
			}
		}
		pictureBox_OutputImage->Image = HighpassImage;
	}
	System::Void MyForm::discreteFTToolStripMenuItem_Click(System::Object ^ sender, System::EventArgs ^ e)
	{
		int w = dataManager->GetImageWidth();
		int h = dataManager->GetImageHeight();

		// 利用傅立葉之平移性，平移頻率
		for (int i = 0; i < h; i++)
		{
			for (int j = 0; j < w; j++)
			{
				int valuePixeli = dataManager->GetInputImage()[i][j];
				valuePixeli = valuePixeli * pow((float)-1, (float)(i + j));
				dataManager->SetPixel(j, i, valuePixeli);
			}
		}

		//將算出頻率資訊傳入輸出影像
		fourierTransformMethod->DiscreteFourierTransform(dataManager->GetInputImage(), dataManager->GetOutputImage(), dataManager->GetFreqReal(), dataManager->GetFreqImag(), h, w);
		Bitmap^ DFTImage = gcnew Bitmap(w, h);
		for (int i = 0; i < h; i++)
		{
			for (int j = 0; j < w; j++)
			{
				int valuePixeli = dataManager->GetOutputImage()[i][j];
				if (valuePixeli > 255)
				{
					valuePixeli = 255;
				}
				else if (valuePixeli < 0)
				{
					valuePixeli = 0;
				}
				DFTImage->SetPixel(j, i, Color::FromArgb(valuePixeli, valuePixeli, valuePixeli));
			}
		}
		pictureBox_OutputImage->Image = DFTImage;
	}
	System::Void MyForm::inverseDiscreteFTToolStripMenuItem_Click(System::Object ^ sender, System::EventArgs ^ e)
	{
		int w = dataManager->GetImageWidth();
		int h = dataManager->GetImageHeight();

		// 利用傅立葉之平移性，平移頻率
		for (int i = 0; i < h; i++)
		{
			for (int j = 0; j < w; j++)
			{
				int valuePixeli = dataManager->GetInputImage()[i][j];
				valuePixeli = valuePixeli * pow((float)-1, (float)(i + j));
				dataManager->SetPixel(j, i, valuePixeli);
			}
		}
		fourierTransformMethod->InverseDiscreteFourierTransform(dataManager->GetInputImage(), dataManager->GetOutputImage(), dataManager->GetFreqReal(), dataManager->GetFreqImag(), h, w);

		//將算出頻率資訊傳入輸出影像
		Bitmap^ IDFTImage = gcnew Bitmap(w, h);
		for (int i = 0; i < h; i++)
		{
			for (int j = 0; j < w; j++)
			{
				int valuePixeli = dataManager->GetOutputImage()[i][j];

				if (valuePixeli > 255)
				{
					valuePixeli = 255;
				}
				else if (valuePixeli < 0)
				{
					valuePixeli = 0;
				}
				IDFTImage->SetPixel(j, i, Color::FromArgb(valuePixeli, valuePixeli, valuePixeli));
			}
		}

		pictureBox_OutputImage->Image = IDFTImage;
	}
	System::Void MyForm::clickHereToolStripMenuItem_Click(System::Object ^ sender, System::EventArgs ^ e)
	{
		// dataManager >> origin image file

		int w = dataManager->GetImageWidth();
		int h = dataManager->GetImageHeight();

		// -- Compute FFT image 
		DataManager FFT = *dataManager;

		// 利用傅立葉之平移性，平移頻率
		for (int i = 0; i < h; i++)
		{
			for (int j = 0; j < w; j++)
			{
				int valuePixeli = FFT.GetInputImage()[i][j];
				valuePixeli = valuePixeli * pow((float)-1, (float)(i + j));
				FFT.SetPixel(j, i, valuePixeli);
			}
		}

		//將算出頻率資訊傳入輸出影像
		fourierTransformMethod->FastFourierTransform(FFT.GetInputImage(), FFT.GetOutputImage(), FFT.GetFreqReal(), FFT.GetFreqImag(), h, w);
		pictureBox_FFT->Image = printToBox(FFT);
		// -- Finish FFT

		// Compute Lowpass 
		DataManager lowpass = FFT;

		// 利用傅立葉之平移性，平移頻率
		for (int i = 0; i < h; i++)
		{
			for (int j = 0; j < w; j++)
			{
				int valuePixeli = lowpass.GetInputImage()[i][j];
				valuePixeli = valuePixeli * pow((float)-1, (float)(i + j));
				lowpass.SetPixel(j, i, valuePixeli);
			}
		}

		//將算出頻率資訊傳入輸出影像
		fourierTransformMethod->LowpassFilter(lowpass.GetFreqReal(), lowpass.GetFreqImag(), lowpass.GetOutputImage(), h, w);
		pictureBox_Lowpass->Image = printToBox(lowpass);

		// Compute Inverse Lowpass Image
		for (int i = 0; i < h; i++)
		{
			for (int j = 0; j < w; j++)
			{
				int valuePixeli = lowpass.GetInputImage()[i][j];
				valuePixeli = valuePixeli * pow((float)-1, (float)(i + j));
				lowpass.SetPixel(j, i, valuePixeli);
			}
		}

		//將算出頻率資訊傳入輸出影像
		fourierTransformMethod->InverseFastFourierTransform(lowpass.GetInputImage(), lowpass.GetOutputImage(), lowpass.GetFreqReal(), lowpass.GetFreqImag(), h, w);
		pictureBox_InvLowpass->Image = printToBox(lowpass);

		// -- Finish Lowpass Filter

		// -- Compute Highpass Filter
		DataManager highpass = FFT;

		// 利用傅立葉之平移性，平移頻率
		for (int i = 0; i < h; i++)
		{
			for (int j = 0; j < w; j++)
			{
				int valuePixeli = highpass.GetInputImage()[i][j];
				valuePixeli = valuePixeli * pow((float)-1, (float)(i + j));
				highpass.SetPixel(j, i, valuePixeli);
			}
		}

		//將算出頻率資訊傳入輸出影像
		fourierTransformMethod->HighpassFilter(highpass.GetFreqReal(), highpass.GetFreqImag(), highpass.GetOutputImage(), h, w);
		pictureBox_Highpass->Image = printToBox(highpass);

		// Compute Inverse Highpass Image
		for (int i = 0; i < h; i++)
		{
			for (int j = 0; j < w; j++)
			{
				int valuePixeli = highpass.GetInputImage()[i][j];
				valuePixeli = valuePixeli * pow((float)-1, (float)(i + j));
				highpass.SetPixel(j, i, valuePixeli);
			}
		}

		//將算出頻率資訊傳入輸出影像
		fourierTransformMethod->InverseFastFourierTransform(highpass.GetInputImage(), highpass.GetOutputImage(), highpass.GetFreqReal(), highpass.GetFreqImag(), h, w);
		pictureBox_InvHighpass->Image = printToBox(highpass);

		// -- Compute Inverse FFT

		// 利用傅立葉之平移性，平移頻率
		for (int i = 0; i < h; i++)
		{
			for (int j = 0; j < w; j++)
			{
				int valuePixeli = FFT.GetInputImage()[i][j];
				valuePixeli = valuePixeli * pow((float)-1, (float)(i + j));
				FFT.SetPixel(j, i, valuePixeli);
			}
		}

		//將算出頻率資訊傳入輸出影像
		fourierTransformMethod->InverseFastFourierTransform(FFT.GetInputImage(), FFT.GetOutputImage(), FFT.GetFreqReal(), FFT.GetFreqImag(), h, w);
		pictureBox_IFFT->Image = printToBox(FFT);
		// -- Finish Inverse FFT
	}
	Bitmap ^ MyForm::printToBox(DataManager & data)
	{
		int w = data.GetImageWidth();
		int h = data.GetImageHeight();
		Bitmap^ result = gcnew Bitmap(w, h);
		for (int i = 0; i < h; i++)
		{
			for (int j = 0; j < w; j++)
			{
				int valuePixeli = data.GetOutputImage()[i][j];
				if (valuePixeli > 255)
				{
					valuePixeli = 255;
				}
				else if (valuePixeli < 0)
				{
					valuePixeli = 0;
				}
				result->SetPixel(j, i, Color::FromArgb(valuePixeli, valuePixeli, valuePixeli));
			}
		}
		return result;
	}
}