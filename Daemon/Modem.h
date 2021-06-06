/*
 *   Copyright (C) 2011-2018,2020,2021 by Jonathan Naylor G4KLX
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef	Modem_H
#define	Modem_H

#include "ModemPort.h"
#include "RingBuffer.h"
#include "Defines.h"
#include "Timer.h"

#include <string>

enum RESP_TYPE_MMDVM {
	RTM_OK,
	RTM_TIMEOUT,
	RTM_ERROR
};

enum SERIAL_STATE {
	SS_START,
	SS_LENGTH1,
	SS_LENGTH2,
	SS_TYPE,
	SS_DATA
};

class CModem {
public:
	CModem(bool rxInvert, bool txInvert, bool pttInvert, unsigned int txDelay, bool trace, bool debug);
	~CModem();

	void setPort(IModemPort* port);
	void setRFParams(unsigned int rxFrequency, int rxOffset, unsigned int txFrequency, int txOffset, int txDCOffset, int rxDCOffset, float rfLevel);
	void setLevels(float rxLevel, float txLevel);

	bool open();

	unsigned int getVersion() const;

	unsigned int readData(unsigned char* data);

	bool hasSpace() const;

	bool hasTX() const;
	bool hasCD() const;

	bool hasError() const;

	bool writeData(const unsigned char* data, unsigned int length);

	void clock(unsigned int ms);

	void close();

private:
	unsigned int               m_protocolVersion;
	bool                       m_rxInvert;
	bool                       m_txInvert;
	bool                       m_pttInvert;
	unsigned int               m_txDelay;
	float                      m_rxLevel;
	float                      m_txLevel;
	float                      m_rfLevel;
	bool                       m_trace;
	bool                       m_debug;
	unsigned int               m_rxFrequency;
	unsigned int               m_txFrequency;
	int                        m_rxDCOffset;
	int                        m_txDCOffset;
	IModemPort*                m_port;
	unsigned char*             m_buffer;
	unsigned int               m_length;
	unsigned int               m_offset;
	SERIAL_STATE               m_state;
	unsigned char              m_type;
	CRingBuffer<unsigned char> m_rxData;
	CRingBuffer<unsigned char> m_txData;
	CTimer                     m_statusTimer;
	CTimer                     m_inactivityTimer;
	CTimer                     m_playoutTimer;
	unsigned int               m_space;
	bool                       m_tx;
	bool                       m_cd;
	bool                       m_error;
	unsigned char              m_mode;

	bool readVersion();
	bool readStatus();
	bool setConfig1();
	bool setConfig2();
	bool setFrequency();

	bool writeConfig();

	void printDebug();

	RESP_TYPE_MMDVM getResponse();
};

#endif
