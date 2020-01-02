#pragma once

#include <bdatypes.h>

// �A���e�i�ݒ�
struct AntennaParam {
	long HighOscillator = -1;	// High��LNB���g�� (MHz)
	long LowOscillator = -1;	// Low��LNB���g�� (MHz)
	long LNBSwitch = -1;		// LNB�ؑ֎��g�� (MHz)
	long Tone = -1;				// �g�[���M�� 0 .. off, 1.. on
	long DiSEqC = -1;			// DiSeqC�w��
};

// �ϒ������ݒ�f�[�^
struct ModulationMethod {
	ModulationType Modulation = ModulationType::BDA_MOD_NOT_SET;								// �ϒ��^�C�v
	FECMethod InnerFEC = FECMethod::BDA_FEC_METHOD_NOT_SET;										// �����O���������^�C�v
	BinaryConvolutionCodeRate InnerFECRate = BinaryConvolutionCodeRate::BDA_BCC_RATE_NOT_SET;	// ����FEC���[�g
	FECMethod OuterFEC = FECMethod::BDA_FEC_METHOD_NOT_SET;										// �O���O���������^�C�v
	BinaryConvolutionCodeRate OuterFECRate = BinaryConvolutionCodeRate::BDA_BCC_RATE_NOT_SET;	// �O��FEC���[�g
	long SymbolRate = -1;																		// �V���{�����[�g
	long BandWidth = -1;																		// �ш敝(MHz)
};

// LockChannel()�Ŏg�p����`���[�j���O�p�����[�^
struct TuningParam {
	long Frequency = -1;												// ���g��(MHz)
	Polarisation Polarisation = Polarisation::BDA_POLARISATION_NOT_SET;	// �M���̕Δg
	AntennaParam Antenna = {};											// �A���e�i�ݒ�f�[�^
	ModulationMethod Modulation = {};									// �ϒ������ݒ�f�[�^
	union {
		long ONID = -1;													// �I���W�i���l�b�g���[�NID
		long PhysicalChannel;											// ATSC / Digital Cable�p
	};
	union {
		long TSID = -1;													// �g�����X�|�[�g�X�g���[��ID
		long Channel;													// ATSC / Digital Cable�p
	};
	union {
		long SID = -1;													// �T�[�r�XID
		long MinorChannel;												// ATSC / Digital Cable�p
	};
	long MajorChannel = -1;												// Digital Cable�p
	long SourceID = -1;													// Digital Cable�p
	DWORD IniSpaceID = 0xFFFFFFFFL;										// ini�t�@�C���œǍ��܂ꂽ�`���[�j���O�X�y�[�X�ԍ�
	DWORD IniChannelID = 0xFFFFFFFFL;									// ini�t�@�C���œǍ��܂ꂽ�`�����l���ԍ�
	BOOL LockTwice = FALSE;												// CH�ؑ֓���������I��2�x�s��
	unsigned int LockTwiceDelay = 100;									// CH�ؑ֓���������I��2�x�s���ꍇ��Delay����(msec)
};
