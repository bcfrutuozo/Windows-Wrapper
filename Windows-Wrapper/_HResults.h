#pragma once

class HResult
{
public:

	// System
	
	static constexpr int COR_E_AMBIGUOUSMATCH = 0x8000211D;
	static constexpr int COR_E_APPDOMAINUNLOADED = 0x80131014;
	static constexpr int COR_E_APPLICATION = 0x80131600;
	static constexpr int COR_E_ARGUMENT = 0x80070057;
	static constexpr int COR_E_ARGUMENTOUTOFRANGE = 0x80131502;
	static constexpr int COR_E_ARITHMETIC = 0x80070216;
	static constexpr int COR_E_ARRAYTYPEMISMATCH = 0x80131503;
	static constexpr int COR_E_BADIMAGEFORMAT = 0x8007000B;
	static constexpr int COR_E_TYPEUNLOADED = 0x80131013;
	static constexpr int COR_E_CANNOTUNLOADAPPDOMAIN = 0x80131015;
	static constexpr int COR_E_COMEMULATE = 0x80131535;
	static constexpr int COR_E_CONTEXTMARSHAL = 0x80131504;
	static constexpr int COR_E_DATAMISALIGNED = 0x80131541;
	static constexpr int COR_E_TIMEOUT = 0x80131505;
	static constexpr int COR_E_CUSTOMATTRIBUTEFORMAT = 0x80131605;
	static constexpr int COR_E_DIVIDEBYZERO = 0x80020012; // DISP_E_DIVBYZERO
	static constexpr int COR_E_DUPLICATEWAITOBJECT = 0x80131529;
	static constexpr int COR_E_EXCEPTION = 0x80131500;
	static constexpr int COR_E_EXECUTIONENGINE = 0x80131506;
	static constexpr int COR_E_FIELDACCESS = 0x80131507;
	static constexpr int COR_E_FORMAT = 0x80131537;
	static constexpr int COR_E_INDEXOUTOFRANGE = 0x80131508;
	static constexpr int COR_E_INSUFFICIENTMEMORY = 0x8013153D;
	static constexpr int COR_E_INSUFFICIENTEXECUTIONSTACK = 0x80131578;
	static constexpr int COR_E_INVALIDCAST = 0x80004002;
	static constexpr int COR_E_INVALIDCOMOBJECT = 0x80131527;
	static constexpr int COR_E_INVALIDFILTERCRITERIA = 0x80131601;
	static constexpr int COR_E_INVALIDOLEVARIANTTYPE = 0x80131531;
	static constexpr int COR_E_INVALIDOPERATION = 0x80131509;
	static constexpr int COR_E_INVALIDPROGRAM = 0x8013153A;
	static constexpr int COR_E_KEYNOTFOUND = 0x80131577;
	static constexpr int COR_E_MARSHALDIRECTIVE = 0x80131535;
	static constexpr int COR_E_MEMBERACCESS = 0x8013151A;
	static constexpr int COR_E_METHODACCESS = 0x80131510;
	static constexpr int COR_E_MISSINGFIELD = 0x80131511;
	static constexpr int COR_E_MISSINGMANIFESTRESOURCE = 0x80131532;
	static constexpr int COR_E_MISSINGMEMBER = 0x80131512;
	static constexpr int COR_E_MISSINGMETHOD = 0x80131513;
	static constexpr int COR_E_MISSINGSATELLITEASSEMBLY = 0x80131536;
	static constexpr int COR_E_MULTICASTNOTSUPPORTED = 0x80131514;
	static constexpr int COR_E_NOTFINITENUMBER = 0x80131528;
	static constexpr int COR_E_PLATFORMNOTSUPPORTED = 0x80131539;
	static constexpr int COR_E_NOTSUPPORTED = 0x80131515;
	static constexpr int COR_E_NULLREFERENCE = 0x80004003;
	static constexpr int COR_E_OBJECTDISPOSED = 0x80131622;
	static constexpr int COR_E_OPERATIONCANCELED = 0x8013153B;
	static constexpr int COR_E_OUTOFMEMORY = 0x8007000E;
	static constexpr int COR_E_OVERFLOW = 0x80131516;
	static constexpr int COR_E_RANK = 0x80131517;
	static constexpr int COR_E_REFLECTIONTYPELOAD = 0x80131602;
	static constexpr int COR_E_RUNTIMEWRAPPED = 0x8013153E;
	static constexpr int COR_E_SAFEARRAYRANKMISMATCH = 0x80131538;
	static constexpr int COR_E_SAFEARRAYTYPEMISMATCH = 0x80131533;
	static constexpr int COR_E_SAFEHANDLEMISSINGATTRIBUTE = 0x80131623;
	static constexpr int COR_E_SECURITY = 0x8013150A;
	static constexpr int COR_E_SERIALIZATION = 0x8013150C;
	static constexpr int COR_E_SEMAPHOREFULL = 0x8013152B;
	static constexpr int COR_E_WAITHANDLECANNOTBEOPENED = 0x8013152C;
	static constexpr int COR_E_ABANDONEDMUTEX = 0x8013152D;
	static constexpr int COR_E_STACKOVERFLOW = 0x800703E9;
	static constexpr int COR_E_SYNCHRONIZATIONLOCK = 0x80131518;
	static constexpr int COR_E_SYSTEM = 0x80131501;
	static constexpr int COR_E_TARGET = 0x80131603;
	static constexpr int COR_E_TARGETINVOCATION = 0x80131604;
	static constexpr int COR_E_TARGETPARAMCOUNT = 0x8002000e;
	static constexpr int COR_E_THREADABORTED = 0x80131530;
	static constexpr int COR_E_THREADINTERRUPTED = 0x80131519;
	static constexpr int COR_E_THREADSTATE = 0x80131520;
	static constexpr int COR_E_THREADSTOP = 0x80131521;
	static constexpr int COR_E_THREADSTART = 0x80131525;
	static constexpr int COR_E_TYPEACCESS = 0x80131543;
	static constexpr int COR_E_TYPEINITIALIZATION = 0x80131534;
	static constexpr int COR_E_TYPELOAD = 0x80131522;
	static constexpr int COR_E_ENTRYPOINTNOTFOUND = 0x80131523;
	static constexpr int COR_E_DLLNOTFOUND = 0x80131524;
	static constexpr int COR_E_UNAUTHORIZEDACCESS = 0x80070005;
	static constexpr int COR_E_UNSUPPORTEDFORMAT = 0x80131523;
	static constexpr int COR_E_VERIFICATION = 0x8013150D;
	static constexpr int COR_E_HOSTPROTECTION = 0x80131640;
	static constexpr int CORSEC_E_MIN_GRANT_FAIL = 0x80131417;
	static constexpr int CORSEC_E_NO_EXEC_PERM = 0x80131418;
	static constexpr int CORSEC_E_POLICY_EXCEPTION = 0x80131416;
	static constexpr int CORSEC_E_XMLSYNTAX = 0x80131418;
	static constexpr int CORSEC_E_CRYPTO = 0x80131430;
	static constexpr int CORSEC_E_CRYPTO_UNEX_OPER = 0x80131431;
	static constexpr int FUSION_E_REF_DEF_MISMATCH = 0x80131040;
	static constexpr int FUSION_E_INVALID_NAME = 0x80131047;
	
	// System.IO
	
	static constexpr int COR_E_ENDOFSTREAM = 0x80070026;  // OS defined
	static constexpr int COR_E_FILELOAD = 0x80131621;
	static constexpr int COR_E_FILENOTFOUND = 0x80070002;
	static constexpr int COR_E_DIRECTORYNOTFOUND = 0x80070003;
	static constexpr int COR_E_PATHTOOLONG = 0x800700CE;
	static constexpr int COR_E_IO = 0x80131620;
};