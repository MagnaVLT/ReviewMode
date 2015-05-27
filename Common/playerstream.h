/**
 *
 * ADTF Demo Source for a player application that uses the harddiskplayer to see what is in a file.
 *
 * @file
 * Copyright &copy; Audi Electronics Venture GmbH. All rights reserved
 *
 * $Author: martin.heimlich $
 * $Date: 2008-07-22 11:00:19 +0200 (Di, 22 Jul 2008) $
 * $Revision: 3883 $
 *
 * @remarks
 *
 */
#include "stdafx.h"

/// reference to global runtime object
//ADTF_DEFINE_RUNTIME()

/**
 *
 * Used to handle the pin events
 *
 */
class cPlayerStreamEventSink : public IPinEventSink
{
    UCOM_OBJECT_IMPL(IID_ADTF_PIN_EVENT_SINK, adtf::IPinEventSink);
private:
    cObjectPtr<adtf::IReferenceClock> m_pClock;
    cObjectPtr<adtf::ISampleStreamExtended> m_pHDStream;

public:
    cPlayerStreamEventSink()
    {
        m_pClock = NULL;
        m_pHDStream = NULL;
    }

    virtual ~cPlayerStreamEventSink()
    {
        m_pClock = NULL;
        m_pHDStream = NULL;
    }

    /**
     * Init
     */
    tResult Init(ISampleStreamExtended* pHDStream)
    {
        RETURN_IF_FAILED(_runtime->GetObject(OID_ADTF_REFERENCE_CLOCK,
                            IID_ADTF_REFERENCE_CLOCK,
                            (tVoid**)&m_pClock));
        m_pHDStream = pHDStream;
        RETURN_NOERROR;
    }

    /**
     *   DeInit
     */
    tResult DeInit()
    {
        m_pClock  = NULL;
        m_pHDStream = NULL;
        RETURN_NOERROR;
    }
    
    /**
     * 
     * This Function will be called by all pins the filter is registered to.
     * @param [in] pSource Pointer to the sending pin's IPin interface.
     * @param [in] nEventCode Event code. 
     * @param [in] nParam1 Optional integer parameter.
     * @param [in] nParam2 Optional integer parameter.
     * @param [in] pMediaSample Address of an IMediaSample interface pointers.
     *
     * @return   Returns a standard result code.
     * @see adtf::cFilter::OnPinEvent
     */
    tResult OnPinEvent(IPin* pSource,
                       tInt nEventCode,
                       tInt nParam1,
                       tInt nParam2,
                       IMediaSample* pMediaSample)
    {
        if (nEventCode == IPinEventSink::PE_MediaSampleTransmitted)
        {
            RETURN_IF_POINTER_NULL(pSource);
            cObjectPtr<IMediaType> pType;

            //to get the type of the current sample
            pSource->GetMediaType(&pType);
            //to get the stream name of the current sample
            //const tChar* strName = pSource->GetName();

            //we can access the sample like we do within a filter
            const tVoid* pBuffer;
            pMediaSample->Lock(&pBuffer);
            pMediaSample->Unlock(pBuffer);

            if (m_pClock && m_pHDStream)
            {
                //normally the player will set the current global time to the simulated chunk time of the file 
                //BUT be Aware! This will only be set to the expected during playing 
                //While using the adtf::ISampleStream::SetCurrentPosition() meore than the expected Sample is transmitted 
                // SEE documentation of adtf::ISampleStream / adtf::ISampleStreamExtended
                tTimeStamp tmCurrentTimePlayerSetGlobal = m_pClock->GetStreamTime();
                tTimeStamp tmSampleTime                 = pMediaSample->GetTime();

                //we will give NO WARRANTY ON THIS calls outside of this example
                //this is a Harddisk Player ONLY example!
                //Only if the Harddisk Player, Reference Clock Service and Kernel Service is part
                //of the system it will work like shown here ! 
                tInt64 tmCurrentTimePosition = m_pHDStream->GetCurrentPosition(ISampleStream::TF_MediaTime);
                tInt64 nCurrentIndexPosition = m_pHDStream->GetCurrentPosition(ISampleStream::TF_Sample);
                if (tmCurrentTimePosition != tmCurrentTimePlayerSetGlobal)
                {
                    
                    //this will happen if somebody called the ISampleStream::SetCurrentPosition without set 
                    // ISampleStream::SF_OnlyPositionTransmit Flag
                    // See documentation of ISampleStream::SetCurrentPosition
                    // another possibility is that you use the message bus or other time synchronizer within
                    // your application.
                    printf(cString::Format("SIM TIME DIFFERENT FROM CURRENT TIME (SimTime %lld) Current Sample played #%lld, Time %lld, Sample Time %lld\n",
                                           tmCurrentTimePlayerSetGlobal,
                                           nCurrentIndexPosition,
                                           tmCurrentTimePosition,
                                           tmSampleTime).GetPtr());
                }
                else
                {
                    printf(cString::Format("(SimTime %lld) Current Sample played #%lld, Time %lld, Sample Time %lld\n",
                                           tmCurrentTimePlayerSetGlobal,
                                           nCurrentIndexPosition,
                                           tmCurrentTimePosition,
                                           tmSampleTime).GetPtr());
                }
            }
        }

        RETURN_NOERROR;
    }
};

