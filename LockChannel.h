#pragma once

#include <bdatypes.h>

// アンテナ設定
struct AntennaParam {
	long HighOscillator = -1;	// High側LNB周波数 (MHz)
	long LowOscillator = -1;	// Low側LNB周波数 (MHz)
	long LNBSwitch = -1;		// LNB切替周波数 (MHz)
	long Tone = -1;				// トーン信号 0 .. off, 1.. on
	long DiSEqC = -1;			// DiSeqC指定
};

// 変調方式設定データ
struct ModulationMethod {
	ModulationType Modulation = ModulationType::BDA_MOD_NOT_SET;								// 変調タイプ
	FECMethod InnerFEC = FECMethod::BDA_FEC_METHOD_NOT_SET;										// 内部前方誤り訂正タイプ
	BinaryConvolutionCodeRate InnerFECRate = BinaryConvolutionCodeRate::BDA_BCC_RATE_NOT_SET;	// 内部FECレート
	FECMethod OuterFEC = FECMethod::BDA_FEC_METHOD_NOT_SET;										// 外部前方誤り訂正タイプ
	BinaryConvolutionCodeRate OuterFECRate = BinaryConvolutionCodeRate::BDA_BCC_RATE_NOT_SET;	// 外部FECレート
	long SymbolRate = -1;																		// シンボルレート
	long BandWidth = -1;																		// 帯域幅(MHz)
};

// LockChannel()で使用するチューニングパラメータ
struct TuningParam {
	long Frequency = -1;												// 周波数(MHz)
	Polarisation Polarisation = Polarisation::BDA_POLARISATION_NOT_SET;	// 信号の偏波
	AntennaParam Antenna = {};											// アンテナ設定データ
	ModulationMethod Modulation = {};									// 変調方式設定データ
	union {
		long ONID = -1;													// オリジナルネットワークID
		long PhysicalChannel;											// ATSC / Digital Cable用
	};
	union {
		long TSID = -1;													// トランスポートストリームID
		long Channel;													// ATSC / Digital Cable用
	};
	union {
		long SID = -1;													// サービスID
		long MinorChannel;												// ATSC / Digital Cable用
	};
	long MajorChannel = -1;												// Digital Cable用
	long SourceID = -1;													// Digital Cable用
	DWORD IniSpaceID = 0xFFFFFFFFL;										// iniファイルで読込まれたチューニングスペース番号
	DWORD IniChannelID = 0xFFFFFFFFL;									// iniファイルで読込まれたチャンネル番号
	BOOL LockTwice = FALSE;												// CH切替動作を強制的に2度行う
	unsigned int LockTwiceDelay = 100;									// CH切替動作を強制的に2度行う場合のDelay時間(msec)
};
