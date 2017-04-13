/**
* @file   TuioInputWrapper.h
* @author Tobias Badura <tobias.badura@uni-ulm.de>
* @date   2017.01.31
*
* @brief  Declaration of the TuioInputWrapper.
*/

#pragma once
#ifdef WITH_TUIO

#include "TuioListener.h"
#include "TuioClient.h"
#include "UdpReceiver.h"


class TuioInputWrapper : public TUIO::TuioListener {

public:
	void addTuioObject(TUIO::TuioObject *tobj);
	void updateTuioObject(TUIO::TuioObject *tobj);
	void removeTuioObject(TUIO::TuioObject *tobj);

	void addTuioCursor(TUIO::TuioCursor *tcur);
	void updateTuioCursor(TUIO::TuioCursor *tcur);
	void removeTuioCursor(TUIO::TuioCursor *tcur);

	void addTuioBlob(TUIO::TuioBlob *tblb);
	void updateTuioBlob(TUIO::TuioBlob *tblb);
	void removeTuioBlob(TUIO::TuioBlob *tblb);

	void refresh(TUIO::TuioTime frameTime);
};


#endif