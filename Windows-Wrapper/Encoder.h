#pragma once

#include "CommonObject.h"
#include "Guid.h"

class Encoder final : public Object
{
private:

	Guid m_Guid;

public:

	constexpr Encoder(Guid guid) : m_Guid(guid) { }

	constexpr Guid GetGuid() const noexcept { return m_Guid; }

    static constexpr  Encoder Compression() { return Encoder(Guid::from_string("e09d739d-ccd4-44ee-8eba-3fbf8be4fc58")); }
    static constexpr  Encoder ColorDepth() { return Encoder(Guid::from_string("66087055-ad66-4c7c-9a18-38a2310b8337")); }
    static constexpr  Encoder ScanMethod() { return Encoder(Guid::from_string("3a4e2661-3109-4e56-8536-42c156e7dcfa")); }
    static constexpr  Encoder Version() { return Encoder(Guid::from_string("24d18c76-814a-41a4-bf53-1c219cccf797")); }
    static constexpr  Encoder RenderMethod() { return Encoder(Guid::from_string("6d42c53a-229a-4825-8bb7-5c99e2b9a8b8")); }
    static constexpr  Encoder Quality() { return Encoder(Guid::from_string("1d5be4b5-fa4a-452d-9cdd-5db35105e7eb")); }
    static constexpr  Encoder Transformation() { return Encoder(Guid::from_string("8d0eb2d1-a58e-4ea8-aa14-108074b7b6f9")); }
    static constexpr  Encoder LuminanceTable() { return Encoder(Guid::from_string("edb33bce-0266-4a77-b904-27216099e717")); }
    static constexpr  Encoder ChrominanceTable() { return Encoder(Guid::from_string("f2e455dc-09b3-4316-8260-676ada32481c")); }
    static constexpr  Encoder SaveFlag() { return Encoder(Guid::from_string("292266fc-ac40-47bf-8cfc-a85b89a655de")); }
};