#include "ListControl.h"

// Singleton ButtonClass
ListControl::ListClass ListControl::ListClass::m_ListClass;

// ProgressBar class declarations
ListControl::ListClass::ListClass() noexcept
	:
	m_Instance(GetModuleHandle(nullptr))
{
	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof(wc);
	wc.style = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = HandleMessageSetup;
	wc.hInstance = GetInstance();
	wc.lpszClassName = GetName();
	RegisterClassEx(&wc);
}

ListControl::ListClass::~ListClass()
{
	UnregisterClass(m_ClassName, GetInstance());
}

const char* ListControl::ListClass::GetName() noexcept
{
	return m_ClassName;
}

HINSTANCE ListControl::ListClass::GetInstance() noexcept
{
	return m_ListClass.m_Instance;
}

int ListControl::OnEraseBackground_Impl(HWND hwnd, HDC hdc) noexcept
{
	return 1;	// To avoid flickering
}

ListControl::ListControl()
	:
	m_DataSource(nullptr),
	OnDataSourceChanged(nullptr),
	OnDisplayMemberChanged(nullptr),
	OnFormat(nullptr),
	OnFormatInfoChanged(nullptr),
	OnFormatStringChanged(nullptr),
	OnFormattingEnabledChanged(nullptr),
	OnSelectedValueChanged(nullptr),
	OnValueMemberChanged(nullptr)
{

}

ListControl::~ListControl()
{
	if (m_DataSource != nullptr)
	{
		delete m_DataSource;
		m_DataSource = nullptr;
	}

	if (OnDataSourceChanged != nullptr) { delete OnDataSourceChanged; OnDataSourceChanged = nullptr; }
	if (OnDisplayMemberChanged != nullptr) { delete OnDisplayMemberChanged; OnDisplayMemberChanged = nullptr; }
	if (OnFormat != nullptr) { delete OnFormat; OnFormat = nullptr; }
	if (OnFormatInfoChanged != nullptr) { delete OnFormatInfoChanged; OnFormatInfoChanged = nullptr; }
	if (OnFormatStringChanged != nullptr) { delete OnFormatStringChanged; OnFormatStringChanged = nullptr; }
	if (OnFormattingEnabledChanged != nullptr) { delete OnFormattingEnabledChanged; OnFormattingEnabledChanged = nullptr; }
	if (OnSelectedValueChanged != nullptr) { delete OnSelectedValueChanged; OnSelectedValueChanged = nullptr; }
	if (OnValueMemberChanged != nullptr) { delete OnValueMemberChanged; OnValueMemberChanged = nullptr; }
}

void ListControl::OnDataSourceChangedSet(const std::function<void(Object*, EventArgs*)>& callback) noexcept
{
	OnDataSourceChanged = new EventHandler("OnDataSourceChanged", callback);
	Events.Register(OnDataSourceChanged);
}

void ListControl::OnDisplayMemberChangedSet(const std::function<void(Object*, EventArgs*)>& callback) noexcept
{
	OnDisplayMemberChanged = new EventHandler("OnDisplayMemberChanged", callback);
	Events.Register(OnDisplayMemberChanged);
}

void ListControl::OnFormatSet(const std::function<void(Object*, ListControlConvertEventArgs*)>& callback) noexcept
{
	OnFormat = new ListControlConvertEventHandler("OnFormat", callback);
	Events.Register(OnFormat);
}

void ListControl::OnFormatInfoChangedSet(const std::function<void(Object*, EventArgs*)>& callback) noexcept
{
	OnFormatInfoChanged = new EventHandler("OnFormatInfoChanged", callback);
	Events.Register(OnFormatInfoChanged);
}

void ListControl::OnFormatStringChangedSet(const std::function<void(Object*, EventArgs*)>& callback) noexcept
{
	OnFormatStringChanged = new EventHandler("OnFormatStringChanged", callback);
	Events.Register(OnFormatStringChanged);
}

void ListControl::OnFormattingEnabledChangedSet(const std::function<void(Object*, EventArgs*)>& callback) noexcept
{
	OnFormattingEnabledChanged = new EventHandler("OnFormattingEnabledChanged", callback);
	Events.Register(OnFormattingEnabledChanged);
}

void ListControl::OnSelectedValueChangedSet(const std::function<void(Object*, EventArgs*)>& callback) noexcept
{
	OnSelectedValueChanged = new EventHandler("OnSelectedValueChanged", callback);
	Events.Register(OnSelectedValueChanged);
}

void ListControl::OnValueMemberChangedSet(const std::function<void(Object*, EventArgs*)>& callback) noexcept
{
	OnValueMemberChanged = new EventHandler("OnValueMemberChanged", callback);
	Events.Register(OnValueMemberChanged);
}

bool ListControl::IsSelectionAllowed()
{
	return m_AllowSelection;
}

void ListControl::EnableSelection() noexcept
{
	m_AllowSelection = true;
}

void ListControl::DisableSelection() noexcept
{
	m_AllowSelection = false;
}

void ListControl::SetDataSource(Object* dataSource) noexcept
{
	if (!dynamic_cast<IList<Object>*>(dataSource))
	{
		throw std::invalid_argument("DataSource type must be inherited from IList or IListSource");
	}

	if (m_DataSource != nullptr)
	{
		delete m_DataSource;
		m_DataSource = nullptr;
	}

	m_DataSource = dataSource;
	Dispatch("OnDataSourceChanged", &ArgsDefault);
	Update();
}

unsigned int ListControl::GetSelectedIndex() const noexcept
{
	return m_SelectedIndex;
}

void ListControl::SetSelectedIndex(unsigned int index) noexcept
{
	m_SelectedIndex = index;
	Update();
}

Object* ListControl::GetSelectedValue() const noexcept
{
	return (*dynamic_cast<IList<Object>*>(m_DataSource))[m_SelectedIndex];
}
