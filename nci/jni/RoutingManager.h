/*
 * Copyright (C) 2013 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
/******************************************************************************
 *
 *  The original Work has been changed by NXP Semiconductors.
 *
 *  Copyright (C) 2015 NXP Semiconductors
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 ******************************************************************************/
/*
 *  Manage the listen-mode routing table.
 */
#pragma once
#include "SyncEvent.h"
#include "NfcJniUtil.h"
#include "RouteDataSet.h"
#include "SecureElement.h"
#include <vector>
extern "C"
{
    #include "nfa_api.h"
    #include "nfa_ee_api.h"
}
#if(NXP_EXTNS == TRUE)
#define TECHNOLOGY_BASED_ROUTING        0x00
#define PROTOCOL_BASED_ROUTING          0x01
#define AID_BASED_ROUTING               0x02
#define AVAILABLE_PROTO_ENTRIES() ((NFA_GetNCIVersion() == NCI_VERSION_2_0)?0x02:0x03)

/*Size of type and length Fields : No of bytes*/
#define TYPE_LENGTH_SIZE                0x02

#define MAX_GET_ROUTING_BUFFER_SIZE     740
#define EE_HCI_DEFAULT_HANDLE           0x401

typedef struct protoroutInfo {
    uint8_t ee_handle;
    tNFA_PROTOCOL_MASK  protocols_switch_on;
    tNFA_PROTOCOL_MASK  protocols_switch_off;
    tNFA_PROTOCOL_MASK  protocols_battery_off;
    tNFA_PROTOCOL_MASK  protocols_screen_lock;
    tNFA_PROTOCOL_MASK  protocols_screen_off;
    tNFA_PROTOCOL_MASK  protocols_screen_off_lock;
}ProtoRoutInfo_t;

typedef struct routeInfo {
    uint8_t num_entries;
    ProtoRoutInfo_t protoInfo[4];
}RouteInfo_t;
#define POWER_STATE_MASK 0xE7
#define HOST_SCREEN_STATE_MASK 0x09
#endif

//FelicaOnHost
typedef struct{

    uint8_t nfcid2Len;
    uint8_t sysCodeLen;
    uint8_t optParamLen;
    uint16_t nfcid2Handle;
    uint8_t sysCode[2];
    uint8_t nfcid2[8];
    uint8_t* optParam;
    uint8_t InUse;

   // Mutex mMutex; /*add if it is required */
}NfcID2_info_t;

typedef struct
{
    uint8_t protocol;
    uint16_t routeLoc;
    uint8_t power;
    uint8_t enable;
} protoEntry_t;

typedef struct
{
    uint8_t technology;
    uint16_t routeLoc;
    uint8_t power;
    uint8_t enable;
} techEntry_t;

typedef struct
{
    uint16_t nfceeID;//ID for the route location
    tNFA_TECHNOLOGY_MASK    tech_switch_on;     /* default routing - technologies switch_on  */
    tNFA_TECHNOLOGY_MASK    tech_switch_off;    /* default routing - technologies switch_off */
    tNFA_TECHNOLOGY_MASK    tech_battery_off;   /* default routing - technologies battery_off*/
    tNFA_TECHNOLOGY_MASK    tech_screen_lock;    /* default routing - technologies screen_lock*/
    tNFA_TECHNOLOGY_MASK    tech_screen_off;   /* default routing - technologies screen_off*/
    tNFA_TECHNOLOGY_MASK    tech_screen_off_lock;  /* default routing - technologies screen_off_lock*/
    tNFA_PROTOCOL_MASK      proto_switch_on;    /* default routing - protocols switch_on     */
    tNFA_PROTOCOL_MASK      proto_switch_off;   /* default routing - protocols switch_off    */
    tNFA_PROTOCOL_MASK      proto_battery_off;  /* default routing - protocols battery_off   */
    tNFA_PROTOCOL_MASK      proto_screen_lock;   /* default routing - protocols screen_lock    */
    tNFA_PROTOCOL_MASK      proto_screen_off;  /* default routing - protocols screen_off  */
    tNFA_PROTOCOL_MASK      proto_screen_off_lock;  /* default routing - protocols screen_off_lock  */

} LmrtEntry_t;

class RoutingManager
{
public:
#if(NXP_EXTNS == TRUE)
    static const int NFA_SET_AID_ROUTING = 4;
    static const int NFA_SET_TECHNOLOGY_ROUTING = 1;
    static const int NFA_SET_PROTOCOL_ROUTING = 2;
    nfc_jni_native_data* mNativeData;
#endif

    static const int ROUTE_HOST = 0;
    static const int ROUTE_ESE = 1;

    static RoutingManager& getInstance ();
    bool initialize(nfc_jni_native_data* native);
#if(NXP_EXTNS == TRUE)
    void setRouting(bool);
    void getRouting();
    void registerProtoRouteEntry(tNFA_HANDLE ee_handle,
                                 tNFA_PROTOCOL_MASK  protocols_switch_on,
                                 tNFA_PROTOCOL_MASK  protocols_switch_off,
                                 tNFA_PROTOCOL_MASK  protocols_battery_off,
                                 tNFA_PROTOCOL_MASK  protocols_screen_lock,
                                 tNFA_PROTOCOL_MASK  protocols_screen_off,
                                 tNFA_PROTOCOL_MASK  protocols_screen_off_lock
                                 );
    void notifyReRoutingEntry();
    void HandleAddNfcID2_Req();
    void HandleRmvNfcID2_Req();
    void setCeRouteStrictDisable(uint32_t state);
    void setDefaultTechRouting (int seId, int tech_switchon,int tech_switchoff);
    void setDefaultProtoRouting (int seId, int proto_switchon,int proto_switchoff);
    void processGetRoutingRsp(tNFA_DM_CBACK_DATA* eventData, uint8_t* sRoutingBuff);
    void nfaEEConnect();
    void nfaEEDisconnect();
    SyncEvent mEEDisconnectEvt;
    bool setRoutingEntry(int type, int value, int route, int power);
    bool clearRoutingEntry(int type);
    bool setDefaultRoute(const int defaultRoute, const int protoRoute, const int techRoute);
    bool clearAidTable ();
    bool removeNfcid2Routing(uint8_t* nfcID2);
    bool addAidRouting(const uint8_t* aid, uint8_t aidLen, int route, int power, int aidInfo);
    int  addNfcid2Routing(uint8_t* nfcid2, uint8_t aidLen,const uint8_t* syscode,
    int  syscodelen,const uint8_t* optparam, int optparamlen);
    void handleSERemovedNtf();
    bool is_ee_recovery_ongoing();
    void setEtsiReaederState(se_rd_req_state_t newState);
    se_rd_req_state_t getEtsiReaederState();
    Rdr_req_ntf_info_t getSwpRrdReqInfo();
#if(NXP_NFCC_HCE_F == TRUE)
    void notifyT3tConfigure();
#endif
#else
    bool addAidRouting(const uint8_t* aid, uint8_t aidLen, int route);
#endif

    bool addApduRouting(uint8_t route, uint8_t powerState,const uint8_t* apduData,
         uint8_t apduDataLen ,const uint8_t* apduMask, uint8_t apduMaskLen);

    bool removeApduRouting(uint8_t apduDataLen, const uint8_t* apduData);
    void setEERecovery(bool value);
    void cleanRouting();
    bool removeAidRouting(const uint8_t* aid, uint8_t aidLen);
    bool commitRouting();
    int registerT3tIdentifier(uint8_t* t3tId, uint8_t t3tIdLen);
    void deregisterT3tIdentifier(int handle);
    void onNfccShutdown();
    int registerJniFunctions (JNIEnv* e);
    void ee_removed_disc_ntf_handler(tNFA_HANDLE handle, tNFA_EE_STATUS status);
    SyncEvent mLmrtEvent;
    SyncEvent mEeSetModeEvent;
    SyncEvent mCeRegisterEvent;//FelicaOnHost
    SyncEvent mCeDeRegisterEvent;
    Mutex  mResetHandlerMutex;
    IntervalTimer LmrtRspTimer;
    SyncEvent mEeUpdateEvent;
    IntervalTimer mNfcFRspTimer;
private:
    RoutingManager();
    ~RoutingManager();
    RoutingManager(const RoutingManager&);
    RoutingManager& operator=(const RoutingManager&);

    void handleData (uint8_t technology, const uint8_t* data, uint32_t dataLen, tNFA_STATUS status);
    void notifyActivated (uint8_t technology);
    void notifyDeactivated (uint8_t technology);
    void notifyLmrtFull();
    void printMemberData(void);
    void extractRouteLocationAndPowerStates(const int defaultRoute, const int protoRoute, const int techRoute);
    uint16_t getUiccRouteLocId(const int route);
    void initialiseTableEntries(void);
    void compileProtoEntries(void);
    void compileTechEntries(void);
    void consolidateProtoEntries(void);
    void consolidateTechEntries(void);
    void setProtoRouting(void);
#if(NXP_EXTNS == TRUE)
    void setEmptyAidEntry(void);
#endif
    void setTechRouting(void);
    void processTechEntriesForFwdfunctionality(void);
    void configureOffHostNfceeTechMask(void);
    void checkProtoSeID(void);
    void dumpTables(int);

    static const int DBG               = true;
    //Currently 4 protocols supported namely T3T, ISO-DEP, ISO-7816, NFC-DEP(taken care internally by the libnfc stack)
    static const int MAX_PROTO_ENTRIES = 0x03;
    static const int PROTO_T3T_IDX     = 0x00;
    static const int PROTO_ISODEP_IDX  = 0x01;
    static const int PROTO_ISO7816_IDX = 0x02;
    //Currently 3 Technologies supported namely A,B,F
    static const int MAX_TECH_ENTRIES  = 0x03;
    static const int TECH_A_IDX        = 0x00;
    static const int TECH_B_IDX        = 0x01;
    static const int TECH_F_IDX        = 0x02;
    //Fixed number of Lmrt entries
    static const int MAX_ROUTE_LOC_ENTRIES  = 0x04;
    //Fixed route location Lmrt index
    static const int ROUTE_LOC_HOST_ID_IDX  = 0x00;
    static const int ROUTE_LOC_ESE_ID_IDX   = 0x01;
    static const int ROUTE_LOC_UICC1_ID_IDX = 0x02;
    static const int ROUTE_LOC_UICC2_ID_IDX = 0x03;
    //Fixed route location Lmrt entries
    static const int ROUTE_LOC_HOST_ID      = 0x400;
    static const int ROUTE_LOC_ESE_ID       = SecureElement::EE_HANDLE_0xF3;
    static const int ROUTE_LOC_UICC1_ID     = 0x402;
    static const int ROUTE_LOC_UICC1_ID_NCI2_0 = 0x480;
    static const int ROUTE_LOC_UICC2_ID     = SecureElement::EE_HANDLE_0xF8;
    // Fixed power states masks
    static const int PWR_SWTCH_ON_SCRN_UNLCK_MASK       = 0x01;
    static const int PWR_SWTCH_OFF_MASK                 = 0x02;
    static const int PWR_BATT_OFF_MASK                  = 0x04;
    static const int PWR_SWTCH_ON_SCRN_LOCK_MASK        = 0x08;
    static const int PWR_SWTCH_ON_SCRN_OFF_MASK         = 0x10;
    static const int PWR_SWTCH_ON_SCRN_OFF_LOCK_MASK    = 0x20;
    // See AidRoutingManager.java for corresponding
    // AID_MATCHING_ constants
    // Every routing table entry is matched exact (BCM20793)
    static const int AID_MATCHING_EXACT_ONLY = 0x00;
    // Every routing table entry can be matched either exact or prefix
    static const int AID_MATCHING_EXACT_OR_PREFIX = 0x01;
    // Every routing table entry is matched as a prefix
    static const int AID_MATCHING_PREFIX_ONLY = 0x02;
    // See AidRoutingManager.java for corresponding
    // AID_MATCHING_ platform constants
    //Behavior as per Android-L, supporting prefix match and full
    //match for both OnHost and OffHost apps.
    static const int AID_MATCHING_L = 0x01;
    //Behavior as per Android-KitKat by NXP, supporting prefix match for
    //OffHost and prefix and full both for OnHost apps.
    static const int AID_MATCHING_K = 0x02;
    static void nfcFRspTimerCb(union sigval);
    static void nfaEeCallback (tNFA_EE_EVT event, tNFA_EE_CBACK_DATA* eventData);
    static void stackCallback (uint8_t event, tNFA_CONN_EVT_DATA* eventData);
    static void nfcFCeCallback (uint8_t event, tNFA_CONN_EVT_DATA* eventData);
    static int com_android_nfc_cardemulation_doGetDefaultRouteDestination (JNIEnv* e);
    static int com_android_nfc_cardemulation_doGetDefaultOffHostRouteDestination (JNIEnv* e);
    static int com_android_nfc_cardemulation_doGetAidMatchingMode (JNIEnv* e);
    static int com_android_nfc_cardemulation_doGetAidMatchingPlatform (JNIEnv* e);

    std::vector<uint8_t> mRxDataBuffer;

    // Fields below are final after initialize()
    //int mDefaultEe;
    int mDefaultEeNfcF;
    int mOffHostEe;
    int mActiveSe;
    int mActiveSeNfcF;
    int mAidMatchingMode;
    int mNfcFOnDhHandle;
    int mAidMatchingPlatform;
    tNFA_TECHNOLOGY_MASK mSeTechMask;
    static const JNINativeMethod sMethods [];
    int mDefaultEe; //since this variable is used in both cases moved out of compiler switch
    int mHostListnTechMask;
    int mUiccListnTechMask;
    int mFwdFuntnEnable;
    static int mChipId;
    SyncEvent mEeRegisterEvent;
    SyncEvent mRoutingEvent;
#if(NXP_EXTNS == TRUE)
    bool mIsDirty;
    protoEntry_t mProtoTableEntries[MAX_PROTO_ENTRIES];
    techEntry_t mTechTableEntries[MAX_TECH_ENTRIES];
    LmrtEntry_t mLmrtEntries[MAX_ROUTE_LOC_ENTRIES];
    uint32_t mCeRouteStrictDisable;
    uint32_t mDefaultIso7816SeID;
    uint32_t mDefaultIso7816Powerstate;
    uint32_t mDefaultIsoDepSeID;
    uint32_t mDefaultIsoDepPowerstate;
    uint32_t mDefaultT3TSeID;
    uint32_t mDefaultT3TPowerstate;
    uint32_t mDefaultTechType;
    uint32_t mDefaultTechASeID;
    uint32_t mDefaultTechAPowerstate;
    uint32_t mDefaultTechBSeID;
    uint32_t mDefaultTechBPowerstate;
    uint32_t mDefaultTechFSeID;
    uint32_t mDefaultTechFPowerstate;
    uint32_t mAddAid;
    uint32_t mTechSupportedByEse;
    uint32_t mTechSupportedByUicc1;
    uint32_t mTechSupportedByUicc2;
#endif
    uint32_t mDefaultHCEFRspTimeout;
};
