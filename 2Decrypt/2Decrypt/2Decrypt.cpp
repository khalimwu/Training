// 2Decrypt.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

template <typename T, unsigned S>
inline unsigned arraysize(const T(&v)[S]) { return S; }


int main()
{
	string freqLang = "TEOAISRHNUCMDLGWFPYKJBVQX";
	string message = "Bgc-bfufb tegaedppqna ql aggv zge xof tegaedppfe'l lgjb.Xof adpf vflqanfe logjbvn'x hf pdwqna d cgebv qn coqro xof tbdkfe ql mjlx d lpdbb tdex.Xof tbdkfe QL XOF HGLL; qx'l kgje vjxk xg fnxfexdqn oqp ge ofe.Zgrjl ql d pdxxfe gz vfrqvqna codx xoqnal kgj def ngx agqna xg vg.Xof rglx gz dvvqna d zfdxjef qln'x mjlx xof xqpf qx xdwfl xg rgvf qx.Xof rglx dblg qnrbjvfl xof dvvqxqgn gz dn ghlxdrbf xg zjxjef fstdnlqgn.Xof xeqrw ql xg tqrw xof zfdxjefl xodx vgn'x zqaox fdro gxofe. - Mgon Rdepdrw. (ccc.adpdljxed.rgp / uqfc / nfcl / 234346 ? utkjpvbjr) (ccc.hedqnkijgxf.rgp / ijgxfl / djxogel / m / mgon_rdepdrw.oxpb)";

	decryptMessage(message, freqLang);
	system("PAUSE");
    return 0;
}


void decryptMessage(string message, string freqLang)
{
	//Cached decrypted message
	string OriginalMessage = message;

	int CharCount[26];

	fill_n(CharCount, arraysize(CharCount), 0);

	int CurrentAscii;

	for (unsigned i = 0; i < message.length(); i++)
	{
		if (isalpha(message[i]))
		{
			CurrentAscii = (int)message[i];

			if (CurrentAscii < 97)
			{
				CurrentAscii = CurrentAscii + 32;
			}
			CharCount[CurrentAscii - 97]++;
		}
	}
	/////CharPosInAlphabet
	int CharPosInAlphabet;
	int CurrentHigherCount = 1;
	int CurrentFreqIndex = 0;

	while (CurrentHigherCount != 0)
	{
		CurrentHigherCount = 0;
		for (unsigned i = 0; i < arraysize(CharCount); i++)
		{
			if (CharCount[i] > CurrentHigherCount)
			{
				CurrentHigherCount = CharCount[i];
				CharPosInAlphabet = i;
			}
		}
		
		if (CurrentHigherCount != 0)
		{
			CharCount[CharPosInAlphabet] = 0;
			for (unsigned i = 0; i < OriginalMessage.length(); i++)
			{
				if (isalpha(OriginalMessage[i]))
				{
					CurrentAscii = (int)OriginalMessage[i];
					if (CurrentAscii < 97)
					{
						CurrentAscii = CurrentAscii + 32;
					}

					if ((CurrentAscii-97) == CharPosInAlphabet)
					{
						message[i] = freqLang[CurrentFreqIndex];
					}
				}
			}
			CurrentFreqIndex++;
		}
	}

	for (unsigned i = 0; i < message.length(); i++)
	{
		cout << message[i];
	}

	cout << "\n";

	return;
}