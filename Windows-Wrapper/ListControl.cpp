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


ListControl::ListItemCollection::ListItemCollection(ListControl* owner)
	:
	Collection(owner)
{

}

ListControl::ListItemCollection::ListItemCollection(ListControl* owner, ListItemCollection& value)
	:
	Collection(owner)
{
	for (const auto& item : value)
	{
		Add(item);
	}
}

ListControl::ListItemCollection::ListItemCollection(ListControl* owner, ListItem* value[])
	:
	Collection(owner)
{
	while (value != nullptr)
	{
		Add(*value);
		++value;
	}
}

ListControl::ListControl(Control* parent, const std::string& name, int width, int x, int y)
	:
	ListControl(parent, name, width, 0, x, y)	// Default control size without font is 9
{

}

ListControl::ListControl(Control* parent, const std::string& name, int width, int height, int x, int y)
	:
	Control(parent, name, width, height, x, y),	// Default control size without font is 9
	m_AllowSelection(true),
	Items(nullptr),
	OnDataSourceChanged(nullptr),
	OnDisplayMemberChanged(nullptr),
	OnFormat(nullptr),
	OnFormatInfoChanged(nullptr),
	OnFormatStringChanged(nullptr),
	OnFormattingEnabledChanged(nullptr),
	OnSelectedValueChanged(nullptr),
	OnValueMemberChanged(nullptr),
	m_SelectedIndex(-1),	// Negative value because positive implies a valid selection
	m_SelectedValue(nullptr)
{
	Initialize();
}

ListControl::~ListControl()
{
	if (Items != nullptr) { delete Items; Items = nullptr; }
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
		ListClass::GetName(),																			// Class name
		Text.c_str(),																						// Window title
		WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_CLIPSIBLINGS,										// Style values
		Location.X,																							// X position
		Location.Y,																							// Y position
		m_Size.Width,																						// Width
		m_Size.Height,																						// Height
		static_cast<HWND>(Parent->Handle.ToPointer()),														// Parent handle
		(HMENU)GetId(),						                								// Menu handle
		ListClass::GetInstance(),																		// Module instance handle
		this																								// Pointer to the button instance to work along with HandleMessageSetup function.
	);

	if (Handle.IsNull())
	{
		throw CTL_LAST_EXCEPT();
	}

	// Set default TextBox margin to 3 pixels
	m_BackgroundColor = Color::Window();
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

void ListControl::SetDataSource(ListItemCollection* const dataSource) noexcept
{
	Items = dataSource;
	Dispatch("OnDataSourceChanged", &ArgsDefault);
	Update();
}

int ListControl::GetSelectedIndex() const noexcept
{
	return m_SelectedIndex;
}

void ListControl::SetSelectedIndex(unsigned int index) noexcept
{
	m_SelectedIndex = index;
	m_SelectedValue = (*Items)[m_SelectedIndex];
	Update();
}

ListItem* ListControl::GetSelectedValue() const noexcept
{
	return (*Items)[m_SelectedIndex];
}

void ListControl::SetSelectedValue(const ListItem& item)
{
	bool err = true;

	for (size_t i = 0; i < Items->GetCount(); ++i)
	{
		const auto& it = (*Items)[i];
		if (it->Id == item.Id && it->Value == item.Value)
		{
			SetSelectedIndex(i);
			err = false;
			break;
		}
	}

	if (err)
	{
		throw std::invalid_argument("ListItem does not exist");
	}
}
