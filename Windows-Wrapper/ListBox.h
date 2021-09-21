#pragma once

#include "ListControl.h"
#include "ArgumentNullException.h"
#include "InvalidOperationException.h"

#include <vector>

class ComboBox;

class ListBox : public ListControl
{
	friend class ComboBox;
	friend class Control;

private:

	//class ItemArray
	//{
	//private:

	//	static int LastMask;

	//	void EnsureSpace(int elements)
	//	{
	//		if (entries == nullptr)
	//		{
	//			//entries = new Entry[(std::max)(elements, 4)];
	//		}
	//		//else if (Count + elements >= entries.Length)
	//		//{
	//		//	//int newLength = (std::max)(entries.Length * 2, entries.Length + elements);
	//		//	//Entry* newEntries = new Entry[newLenght];
	//		//	//entries->CopyTo(newEntries, 0);
	//		//	//entries = newEntries;
	//		//}
	//	}

	//	void AddRange(ICollection<>* const items)
	//	{
	//		if (items == nullptr)
	//		{
	//			throw ArgumentNullException("items");
	//		}

	//		EnsureSpace(items->GetCount());

	//		for (const auto& i : *items)
	//		{
	//			entries[Count++] = new Entry(i);
	//		}
	//	}

	//	void Clear()
	//	{
	//		if (Count > 0)
	//		{
	//			//TODO: ARRAY CLEAR
	//		}

	//		Count = 0;
	//		Version++;
	//	}

	//	static int CreateMask()
	//	{
	//		int mask = LastMask;
	//		LastMask = LastMask << 1;
	//		assert(LastMask > mask, "We have overflowed our state mask.");
	//		return mask;
	//	}

	//public:

	//	/*class Entry : public Object
	//	{
	//	public:

	//		Object* Item;
	//		int State;

	//		Entry(Object* const item)
	//			:
	//			Item(item),
	//			State(false)
	//		{

	//		}
	//	};

	//	class EntryEnumerator : public Object, public IEnumeratorBase
	//	{
	//	public:

	//		ItemArray* Items;
	//		bool AnyBit;
	//		int Version;
	//		int Current;
	//		int State;

	//		EntryEnumerator(ItemArray* items, int state, bool anyBit)
	//			:
	//			Items(items),
	//			State(state),
	//			AnyBit(anyBit),
	//			Version(items->Version),
	//			Current(-1)
	//		{

	//		}

	//		bool MoveNext() noexcept override
	//		{
	//			if (Version != Items->Version)
	//			{
	//				throw InvalidOperationException();
	//			}

	//			while (true)
	//			{
	//				if (Current < Items->Count - 1)
	//				{
	//					++Current;
	//					if (AnyBit)
	//					{
	//						if ((Items->entries[Current].State & State) != 0)
	//						{
	//							return true;
	//						}
	//					}
	//					else
	//					{
	//						if ((Items->entries[Current].State & State) == State)
	//						{
	//							return true;
	//						}
	//					}
	//				}
	//				else
	//				{
	//					Current = Items->Count;
	//					return false;
	//				}
	//			}
	//		}

	//		void Reset() noexcept override
	//		{
	//			if (Version != Items->Version)
	//			{
	//				throw InvalidOperationException();
	//			}

	//			Current = -1;
	//		}

	//		Object* GetCurrent() const noexcept override
	//		{
	//			if (Current == -1 || Current == Items->Count)
	//			{
	//				throw InvalidOperationException();
	//			}

	//			return Items->entries[Current].Item;
	//		}
	//	};*/

	//	int Count;
	//	int Version;
	//	ListControl* m_ListControl;
	//	Entry entries[];

	//	ItemArray(ListControl* listControl)
	//		:
	//		m_ListControl(listControl)
	//	{	}

	//	Object* Add(Object* item)
	//	{
	//		EnsureSpace(1);
	//		Version++;
	//		entries[Count] = new Entry(item);
	//		return &entries[Count++];
	//	}
	//};

	

	class SelectedIndexCollection : Collection<int>
	{
		friend class ListBox;

	private:

		ListBox* Owner;

		SelectedIndexCollection(ListBox* owner);
	};

	class SelectedObjectCollection : Collection<>
	{
		friend class ListBox;

	private:

		ListBox* Owner;
		bool StateDirty;
		int LastVersion;

		SelectedObjectCollection(ListBox* owner);

		void ClearSelected() noexcept;
		bool GetSelected(int index);
		void SetSelected(int index, bool isSelected);
	};

	bool m_IsMultiColumn;
	bool m_IsHorizontalScrollVisible;
	bool m_IsScrollAlwaysVisible;
	bool m_IsFormatChanged;
	SelectionMode m_SelectionMode;
	DockStyle m_DockStyle;
	BorderStyle m_BorderStyle;
	std::vector<RECT> m_RowPosition;
	size_t m_TotalItemsInDrawableArea;
	size_t m_ColumnWidth;
	size_t m_RowNumber;
	size_t m_ColumnNumber;
	ListItem* m_SelectedItem;
	int m_CountSelection;
	SelectedIndexCollection m_SelectedIndices;
	SelectedObjectCollection m_SelectedItems;

	void OnKeyDown_Impl(HWND hwnd, unsigned int vk, int cRepeat, unsigned int flags) noexcept override;
	void OnMouseLeftDown_Impl(HWND hwnd, int x, int y, unsigned int keyFlags) noexcept override;
	void OnPaint_Impl(HWND hwnd) noexcept override;

	void CalculateListBoxParameters(HWND hwnd, HDC& hdc);
	void Draw(HWND hwnd, HDC& hdc);
	int GetCountSelected() const noexcept;

	ListBox(Control* parent, int width, int height, int x, int y);


public:

	virtual ~ListBox();

	void SetSelectedIndex(int index) noexcept override;
	void SetSelectedValue(const ListItem& item) override;

	bool IsMultiColumn() const noexcept;
	void EnableMultiColumn() noexcept;
	void DisableMultiColumn() noexcept;
	size_t GetColumnWidth() noexcept;
	void SetColumnWidth(const size_t& width) noexcept;
	BorderStyle GetBorderStyle() const noexcept;
	void SetBorderStyle(BorderStyle style) noexcept;
	SelectionMode GetSelectionMode() const noexcept;
	void SetSelectionMode(SelectionMode mode) noexcept;
	void SelectAll() noexcept;
};