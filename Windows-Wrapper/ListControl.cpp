#include "ListControl.h"
#include "ComboBox.h"

int ListControl::OnEraseBackground_Impl(HWND hwnd, HDC hdc)
{
	return 1;	// To avoid flickering
}

ListControl::ListControl(Control* parent, const std::string& name, int width, int x, int y)
	:
	ListControl(parent, name, width, 0, x, y)	// Default control size without font is 9
{

}

ListControl::ListControl(Control* parent, const std::string& name, int width, int height, int x, int y)
	:
	ScrollableControl(parent, name, width, height, x, y),	// Default control size without font is 9
	m_AllowSelection(true),
	OnDataSourceChanged(nullptr),
	OnDisplayMemberChanged(nullptr),
	OnFormat(nullptr),
	OnFormatInfoChanged(nullptr),
	OnFormatStringChanged(nullptr),
	OnFormattingEnabledChanged(nullptr),
	OnSelectedValueChanged(nullptr),
	OnValueMemberChanged(nullptr),
	m_SelectedIndex(-1),	// Negative value because positive implies a valid selection
	m_SelectedValue(""),
	m_IsRebinding(false)
{
	Initialize();
}

ListControl::~ListControl()
{
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

void ListControl::Initialize()
{
	// Create window and get its handle
	Handle = CreateWindow(
		WindowClass::GetName(),									// Class name
		Text.c_str(),											// Window title
		WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_CLIPSIBLINGS,	// Style values
		m_Location.X,											// X position
		m_Location.Y,											// Y position
		m_Size.Width,											// Width
		m_Size.Height,											// Height
		static_cast<HWND>(Parent->Handle.ToPointer()),			// Parent handle
		nullptr,						                		// Menu handle
		WindowClass::GetInstance(),								// Module instance handle
		this													// Pointer to the class instance to work along with HandleMessageSetup function.
	);

	if (Handle.IsNull())
	{
		throw CTL_LAST_EXCEPT();
	}

	// Initialize scrollbars after control creation and hide it as default
	HorizontalScrollBar.Initialize();
	HorizontalScrollBar.Hide();
	VerticalScrollBar.Initialize();
	VerticalScrollBar.Hide();
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

const std::vector<ListItem>& ListControl::GetDataSource() const noexcept
{
	return Items;
}

void ListControl::SetDataSource(const std::vector<ListItem>& dataSource)
{
	if (dataSource.size() > 32767)
	{
		throw ArgumentException("Maximum ListBox items allowed is 32767");
	}

	// Destroy the old DataSource to avoid memory leak
	Items.clear();

	SetSelectedIndex(-1);

	Items = dataSource;
	Dispatch("OnDataSourceChanged", &ArgsDefault);
	m_IsRebinding = true;
	Update();
}

int ListControl::GetSelectedIndex() const noexcept
{
	return m_SelectedIndex;
}

std::string ListControl::GetSelectedValue() const noexcept
{
	return m_SelectedValue;
}

bool ListControl::IsRebinding() const noexcept
{
	return m_IsRebinding;
}