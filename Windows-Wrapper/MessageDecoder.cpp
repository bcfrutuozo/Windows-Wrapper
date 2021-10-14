#include "MessageDecoder.h"
#include "Message.h"

#include <sstream>

std::string MessageDecoder::ToString(const Message* message) noexcept
{
	return ToString(message->hWnd, message->Msg, message->wParam, message->lParam, message->Result);
}

std::string MessageDecoder::ToString(IntPtr hWnd, int msg, IntPtr wParam, IntPtr lParam, IntPtr result) noexcept
{
    std::string ID = Parenthesize(MsgToString(msg));

    std::string lDescription = "";
    if (msg == WM_PARENTNOTIFY) lDescription = Parenthesize(MsgToString(LOWORD(wParam.ToInt32())));

    std::ostringstream oss;

    oss << "Msg=0x" << std::hex << msg << ID
        << "hWnd=0x" << std::hex << hWnd.ToInt64()
        << "wParam=0x" << std::hex << wParam.ToInt64()
        << "lParam=0x" << std::hex << lParam.ToInt64() << lDescription
        << "Result=0x" << std::hex << result.ToInt64();

    return oss.str();
}
