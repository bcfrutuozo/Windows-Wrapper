#include "Message.h"
#include "MessageDecoder.h"

Message Message::Create(IntPtr hwnd, int msg, IntPtr wparam, IntPtr lparam) noexcept
{
	Message m;

	m.hWnd = hwnd;
	m.Msg = msg;
	m.wParam = wparam;
	m.lParam = lparam;
	m.Result = IntPtr::Zero();

	return m;
}

bool Message::Equals(const Object* const b) const
{
	if (b == nullptr) return false;
	
	if (const auto m = dynamic_cast<const Message*>(b))
	{
		return hWnd == m->hWnd &&
				Msg == m->Msg &&
				wParam == m->wParam &&
				lParam == m->lParam &&
				Result == m->Result;
	}

	return false;
}

int Message::GetHashCode() const noexcept
{
	return static_cast<int>(hWnd.ToInt32()) << 4 | Msg;
}

std::string Message::ToString() const noexcept
{
	return MessageDecoder::ToString(this);
}