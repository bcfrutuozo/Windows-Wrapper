#include "TestUserApplication.h"
#include "KeyEventArgs.h"
#include "ToolStrip.h"
#include "ListBox.h"
#include "Color.h"
#include "Boolean.h"
#include "Byte.h"
#include "Int16.h"
#include "UInt16.h"
#include "Int32.h"
#include "SByte.h"
#include "Char.h"
#include "UInt32.h"
#include "Int64.h"
#include "UInt64.h"
#include "Single.h"
#include "Double.h"

#include "Event.h"
#include "Timer.h"

void TestClick(Object* sender, EventArgs* e)
{
	Button* b = (Button*)sender;
    printf_s("AOOO CRICOUU");
}

void TestUserApplication::Initialize()
{   
    //
    //// Verify the underlying type is exposed by the value_type member.
    static_assert(std::is_same<int, typename Int32::value_type >::value, "Value type not exposed.");
    //
    //// Without providing a value, primitives should be zeroed out.
    Int32 uninitialized;
    assert(uninitialized.Get() == 0);
    static_assert(Int32().Get() == 0, "The value was not default-initialized from the default ctor.");
    //
    //// Check the provided value is stored.
    static_assert(Int32(42).Get() == 42, "The value was not initialized from a value.");
    //
    //// Check the copy ctor copies the underlying value.
    constexpr Int32 source(42);
    static_assert(Int32(source).Get() == 42, "The value was not copied in the copy ctor.");
    //
    //// Check the move ctor moves the underlying value.
    static_assert(Int32(std::move(Int32(42))).Get() == 42, "The value was not moved in the move ctor.");
    //
    //// Check the copy operator copies the underlying value.
    Int32 copyDestination;
    copyDestination = Int32(33);
    assert(copyDestination.Get() == 33);
    //
    //// Check the move operator moves the underlying value.__construct_from_string_literal
    Int32 moveDestination;
    moveDestination = std::move(Int32(33));
    assert(moveDestination.Get() == 33);
    //
    //// Unary operators
    constexpr Int32 pos(3);
    constexpr Int32 pos_res = +pos;
    static_assert(pos.Get() == pos_res.Get(), "The + unary operator did not return the primitive.");
    //
    constexpr Int32 neg(5);
    constexpr Int32 neg_res = -neg;
    static_assert(-neg.Get() == neg_res.Get(), "The - unary operator did not negate the number.");
    //
    constexpr Int32 bitneg(10);
    constexpr Int32 bitneg_res = ~bitneg;
    static_assert(~bitneg.Get() == bitneg_res.Get(), "The ~ unary operator did not bitwise negate the number.");
    //
    constexpr Boolean logicalneg(false);
    constexpr Boolean logicalneg_res(!logicalneg);
    static_assert(!logicalneg.Get() == logicalneg_res.Get(), "The ! unary operator did not logically negate the value.");
    //
    static_assert(!!Boolean(true), "The value did not convert to true.");
    static_assert(!Boolean(false), "The value did convert to false.");
    //
    Int32 preinc(4);
    ++preinc;
    assert(preinc.Get() == 5);
    //
    Int32 postinc(4);
    Int32 postinc_res = postinc++;
    assert(postinc.Get() == 5);
    assert(postinc_res.Get() == 4);
    //
    Int32 predec(4);
    --predec;
    assert(predec.Get() == 3);
    //
    Int32 postdec(4);
    Int32 postdec_res = postdec--;
    assert(postdec.Get() == 3);
    assert(postdec_res.Get() == 4);
    //
    // Assignment operators
    Int32 add(5);
    add += 4;
    assert(add.Get() == 9);
    
    Int32 add_self(5);
    add_self += Int16::From(4);
    assert(add_self.Get() == 9);
    
    Int32 substract(5);
    substract -= 4;
    assert(substract.Get() == 1);
    
    Int32 substract_self(5);
    substract_self -= Int16::From(4);
    assert(substract_self.Get() == 1);
    
    Int32 multiply(5);
    multiply *= 4;
    assert(multiply.Get() == 20);
    
    Int32 multiply_self(5);
    multiply_self *= Int16::From(4);
    assert(multiply_self.Get() == 20);
    
    Int32 divide(20);
    divide /= 4;
    assert(divide.Get() == 5);
    
    Int32 divide_self(20);
    divide_self /= Int16::From(4);
    assert(divide_self.Get() == 5);
    
    Int32 modulus(21);
    modulus %= 4;
    assert(modulus.Get() == 1);
    
    Int32 modulus_self(21);
    modulus_self %= Int16::From(4);
    assert(modulus_self.Get() == 1);
    
    Int32 shiftleft(2);
    shiftleft <<= 1;
    assert(shiftleft.Get() == 4);
    
    Int32 shiftleft_self(2);
    shiftleft_self <<= 1;
    assert(shiftleft_self.Get() == 4);
    
    Int32 shiftright(4);
    shiftright >>= 1;
    assert(shiftright.Get() == 2);
    
    Int32 shiftright_self(4);
    shiftright_self >>= 1;
    assert(shiftright_self.Get() == 2);
    
    Int32 bit_and(5);
    bit_and &= 1;
    assert(bit_and.Get() == 1);
    
    Int32 bitand_self(5);
    bitand_self &= 1;
    assert(bitand_self.Get() == 1);
    
    Int32 bit_or(5);
    bit_or |= 2;
    assert(bit_or.Get() == 7);
    
    Int32 bitor_self(5);
    bitor_self |= 2;
    assert(bitor_self.Get() == 7);
    
    Int32 bitxor(7);
    bitxor ^= 2;
    assert(bitxor.Get() == 5);

    Int32 bitxor_self(7);
    bitxor_self ^= 2;
    assert(bitxor_self.Get() == 5);
    //
    //// Implicit conversions
    constexpr Int16 schar2shortconv(SByte::From(12));
    static_assert(schar2shortconv.Get() == 12, "A signed char was not converted to a short.");
    constexpr UInt16 uchar2ushortconv(Byte::From(12u));
    static_assert(uchar2ushortconv.Get() == 12, "An unsigned char was not converted to an unsigned short.");
    //
    constexpr Int32 schar2intconv(SByte::From(12));
    static_assert(schar2intconv.Get() == 12, "A signed char was not converted to an int.");
    ;
    constexpr Int32 short2intconv(Byte::From(12));
    static_assert(short2intconv.Get() == 12, "A signed short was not converted to an int.");
    //
    constexpr UInt32 uchar2uintconv(Byte::From(12u));
    static_assert(uchar2uintconv.Get() == 12, "An unsigned char was not converted to an unsigned int.");
    constexpr UInt32 ushort2uintconv(UInt16::From(12u));
    static_assert(ushort2uintconv.Get() == 12, "An unsigned short was not converted to an unsigned int.");
    //
    constexpr Int64 schar2longconv(SByte::From(12));
    static_assert(schar2longconv.Get() == 12, "A signed char was not converted to a long.");
    constexpr Int64 short2longconv(Int16::From(12));
    static_assert(short2longconv.Get() == 12, "A signed short was not converted to a long.");
    constexpr Int64 int2longconv(Int32(12));
    static_assert(int2longconv.Get() == 12, "A signed int was not converted to a long.");
    //
    constexpr UInt64 uchar2ulongconv(Byte::From(12u));
    static_assert(uchar2ulongconv.Get() == 12, "An unsigned char was not converted to an unsigned long.");
    constexpr UInt64 ushort2ulongconv(UInt32::From(12u));
    static_assert(ushort2ulongconv.Get() == 12, "An unsigned short was not converted to an unsigned long.");
    constexpr UInt64 uint2ulongconv(UInt32(12u));
    static_assert(uint2ulongconv.Get() == 12, "An unsigned int was not converted to an unsigned long.");
    //
    //
    constexpr Single schar2doubleconv(SByte::From(-12));
    static_assert(schar2doubleconv.Get() == -12.0, "A signed char was not converted to a double.");
    constexpr Single uchar2doubleconv(Byte::From(12u));
    static_assert(uchar2doubleconv.Get() == 12.0, "An unsigned char was not converted to a double.");
    constexpr Single short2doubleconv(Int16::From(-12));
    static_assert(short2doubleconv.Get() == -12.0, "A signed short was not converted to a double.");
    constexpr Single ushort2doubleconv(UInt16::From(12u));
    static_assert(ushort2doubleconv.Get() == 12.0, "An unsigned short was not converted to a double.");
    constexpr Single int2doubleconv(Int32(-12));
    static_assert(int2doubleconv.Get() == -12.0, "A signed int was not converted to a double.");
    constexpr Single uint2doubleconv(UInt32(12u));
    static_assert(uint2doubleconv.Get() == 12.0, "An unsigned int was not converted to a double.");
    constexpr Single long2doubleconv(Int64(-12));
    static_assert(long2doubleconv.Get() == -12.0, "A signed long was not converted to a double.");
    constexpr Single ulong2doubleconv(UInt64(12u));
    static_assert(ulong2doubleconv.Get() == 12.0, "An unsigned long was not converted to a double.");
    constexpr Single float2doubleconv(Double(-12.0f));
    static_assert(float2doubleconv.Get() == -12.0, "A signed long was not converted to a double.");
    //
    constexpr Double schar2longdoubleconv(SByte::From(-12));
    static_assert(schar2longdoubleconv.Get() == -12.0, "A signed char was not converted to a long double.");
    constexpr Double uchar2longdoubleconv(Byte::From(12u));
    static_assert(uchar2longdoubleconv.Get() == 12.0, "An unsigned char was not converted to a long double.");
    constexpr Double short2longdoubleconv(Int16::From(-12));
    static_assert(short2longdoubleconv.Get() == -12.0, "A signed short was not converted to a long double.");
    constexpr Double ushort2longdoubleconv(UInt16::From(12u));
    static_assert(ushort2longdoubleconv.Get() == 12.0, "An unsigned short was not converted to a long double.");
    constexpr Double int2longdoubleconv(Int32(-12));
    static_assert(int2longdoubleconv.Get() == -12.0, "A signed int was not converted to a long double.");
    constexpr Double uint2longdoubleconv(UInt32(12u));
    static_assert(uint2longdoubleconv.Get() == 12.0, "An unsigned signed int was not converted to a long double.");
    constexpr Double long2longdoubleconv(Int64(-12));
    static_assert(long2longdoubleconv.Get() == -12.0, "A signed long was not converted to a long double.");
    constexpr Double ulong2longdoubleconv(UInt64(12u));
    static_assert(ulong2longdoubleconv.Get() == 12.0, "An unsigned signed long was not converted to a long double.");
    constexpr Double float2longdoubleconv(Double(-12.0f));
    static_assert(float2longdoubleconv.Get() == -12.0, "A float was not converted to a long double.");
    constexpr Double double2longdoubleconv(Single(-12));
    static_assert(double2longdoubleconv.Get() == -12.0, "A double was not converted to a long double.");
    //
    //// Explicit conversions
    constexpr Single double2float = static_cast<Single>(Double(12));
    static_assert(double2float.Get() == 12, "A double was not converted explicitly to a float.");
    //
    //// Binary Operators
    static_assert((Int32(1) + 1).Get() == 2, "Binary + failed.");
    static_assert((1 + Int32(1)).Get() == 2, "Binary + failed.");
    static_assert((Int32(1) + Int32(1)).Get() == 2, "Binary + failed.");
    //
    static_assert((Int32(2) - 1).Get() == 1, "Binary - failed.");
    static_assert((2 - Int32(1)).Get() == 1, "Binary - failed.");
    static_assert((Int32(2) - Int32(1)).Get() == 1, "Binary - failed.");
    //
    static_assert((Int32(2) * 3).Get() == 6, "Binary * failed.");
    static_assert((2 * Int32(3)).Get() == 6, "Binary * failed.");
    static_assert((Int32(2) * Int32(3)).Get() == 6, "Binary * failed.");
    //
    static_assert((Int32(6) / 3).Get() == 2, "Binary / failed.");
    static_assert((6 / Int32(3)).Get() == 2, "Binary / failed.");
    static_assert((Int32(6) / Int32(3)).Get() == 2, "Binary / failed.");
    //
    static_assert((Int32(6) % 5).Get() == 1, "Binary % failed.");
    static_assert((6 % Int32(5)).Get() == 1, "Binary % failed.");
    static_assert((Int32(6) % Int32(5)).Get() == 1, "Binary % failed.");
    //
    static_assert((Int32(5) & 3).Get() == 1, "Binary & failed.");
    static_assert((5 & Int32(3)).Get() == 1, "Binary & failed.");
    static_assert((Int32(5) & Int32(3)).Get() == 1, "Binary & failed.");
    //
    static_assert((Int32(5) | 2).Get() == 7, "Binary | failed.");
    static_assert((5 | Int32(2)).Get() == 7, "Binary | failed.");
    static_assert((Int32(5) | Int32(2)).Get() == 7, "Binary | failed.");
    //
    static_assert((Int32(7) ^ 2).Get() == 5, "Binary ^ failed.");
    static_assert((7 ^ Int32(2)).Get() == 5, "Binary ^ failed.");
    static_assert((Int32(7) ^ Int32(2)).Get() == 5, "Binary ^ failed.");
    //
    static_assert((Int32(2) << 1).Get() == 4, "Binary << failed.");
    static_assert((2 << Int32(1)).Get() == 4, "Binary << failed.");
    static_assert((Int32(2) << Int32(1)).Get() == 4, "Binary << failed.");
    //
    static_assert((Int32(4) >> 1).Get() == 2, "Binary >> failed.");
    static_assert((4 >> Int32(1)).Get() == 2, "Binary >> failed.");
    static_assert((Int32(4) >> Int32(1)).Get() == 2, "Binary >> failed.");
    //
    static_assert(!(Boolean(true) && false), "Binary && failed.");
    static_assert(!(true && Boolean(false)), "Binary && failed.");
    static_assert(!(Boolean(true) && Boolean(false)), "Binary && failed.");
    //
    static_assert((Boolean(true) || false), "Binary || failed.");
    static_assert((true || Boolean(false)), "Binary || failed.");
    static_assert((Boolean(true) || Boolean(false)), "Binary || failed.");
    //
    static_assert(Int32(1) == 1, "Binary == failed.");
    static_assert(1 == Int32(1), "Binary == failed.");
    static_assert(Int32(1) == Int32(1), "Binary == failed.");
    //
    static_assert(Int32(1) != 2, "Binary != failed.");
    static_assert(1 != Int32(2), "Binary != failed.");
    static_assert(Int32(1) != Int32(2), "Binary != failed.");
    //
    static_assert(Int32(1) < 2, "Binary < failed.");
    static_assert(1 < Int32(2), "Binary < failed.");
    static_assert(Int32(1) < Int32(2), "Binary < failed.");
    //
    static_assert(Int32(1) <= 2, "Binary <= failed.");
    static_assert(1 <= Int32(2), "Binary <= failed.");
    static_assert(Int32(1) <= Int32(2), "Binary <= failed.");
    //
    static_assert(Int32(3) > 2, "Binary > failed.");
    static_assert(3 > Int32(2), "Binary > failed.");
    static_assert(Int32(3) > Int32(2), "Binary > failed.");
    //
    static_assert(Int32(3) >= 2, "Binary >= failed.");
    static_assert(3 >= Int32(2), "Binary >= failed.");
    static_assert(Int32(3) >= Int32(2), "Binary >= failed.");
    //
    //// Test iostreams
    Int32 cinned;
    std::istringstream input("123");
    input >> cinned;
    assert(cinned == 123);
    //
    constexpr Int32 couted(123);
    std::ostringstream output;
    output << couted;
    assert(output.str() == "123");
    //
    //// Test promotions
    constexpr Int32 expanded = Byte::From(123);
    static_assert(expanded.Get() == 123, "Could not convert from short to int.");
    //
    Int32 expand_into;
    expand_into = Byte::From(100);
    assert(expand_into == 100);
    //
    //// Test demotions
    static_assert(SByte::From(12).Get() == 12, "Could not create a signed char from an int.");
    static_assert(Byte::From(12u).Get() == 12, "Could not create an unsigned char from an unsigned int.");
    static_assert(Int16::From(12).Get() == 12, "Could not create a short from an int.");
    static_assert(UInt16::From(12u).Get() == 12, "Could not create an unsigned short from an unsigned int.");
    //
    SByte tester = SByte::From(100);
    tester = SByte::From(500);
    //
    constexpr Char character = 'a';
    static_assert(character.Get() == 'a', "Could not read a character.");
























	Window* window = new Window("Teste", 1000, 1000);
	window->GetMouse().EnableRaw();
	window->GetKeyboard().DisableAutorepeat();
	//
	auto lb = window->AddListBox(260, 240, 450, 50);
	//
	auto list = new ListItemCollection(lb);
	for (size_t i = 0; i < 1000; ++i)
	{
		std::ostringstream oss;
		oss << "Item: " << i;
		list->Add(new ListItem(i, oss.str()));
	}
	lb->SetDataSource(list);
	auto f = lb->GetFont();
	f.SetStyle(FontStyle::Italic);
	f.SetSize(14);
	lb->SetFont(f);
	lb->SetBorderStyle(BorderStyle::Fixed3D);
	//lb->EnableMultiColumn();
	lb->SetColumnWidth(180);
	//
	auto cb = window->AddComboBox("Start Text", 260, 450, 450);
	// Double list to check for memory leak during DataSource rebinding
	auto list2 = new ListItemCollection(lb);
	for (size_t i = 0; i < 5; ++i)
	{
		std::ostringstream oss;
		oss << "Item: " << i;
		list2->Add(new ListItem(i, oss.str()));
	}
	cb->SetDataSource(list2);
	//
	////auto t1 = window->AddComboBox("ComboBox", 300, 120, 120);
	////window2->AddButton("Teste", 250, 250, 125, 125);
	//
	////ToolStrip& p = window->AddToolStrip();
	////ToolStrip& j = window->AddToolStrip();
	////p.SetBackgroundColor(Color::Black());
	////j.SetBackgroundColor(Color::Blue());
	////auto l = window->AddLabel("abcdef", 300, 100);
	////auto f = l->GetFont();
	////f.SetSize(16);
	////f.SetStyle(FontStyle::Bold);
	////f.SetStyle(FontStyle::Strikeout);
	////l->SetBorderStyle(BorderStyle::None);
	////l->SetFont(f);
	////
	//button1 = window->AddButton("Abc", 200, 80, 50, 100);
	//button1->OnClickSet([](Object* sender, EventArgs* e)
	//	{
	//		printf_s("Button1 Click Test");
	//	}
	//);
	//
	//txtBox1 = window->AddTextBox("TextBox", 400, 20, 400);
	//
	//////auto& mb = window->GetMenuStrip();
	//////auto& item1 = mb.AddItem("Arquivo");
	//////auto& item2 = mb.AddItem("Arquivo2");
	//////auto& menu1 = mb.AddMenu("Submenu");
	//////menu1.AddItem("Submenu-Item1").OnClickSet(&TestClick);
	//////menu1.AddCheckItem("Check-test01", true).OnClickSet(&TestClick);
	//////menu1.AddCheckItem("Check-test02", false).OnClickSet(&TestClick);
	//////menu1.AddSeparator();
	//////menu1.AddItemWithIcon("Submenu-Item2", "images\\1.bmp").OnClickSet(&TestClick);
	//////menu1.AddSeparator();
	//////auto& menu2 = menu1.AddMenu("Submenu-Item3");
	//////menu2.AddItemWithIcon("Submenu2-Item1", "images\\2.bmp").OnClickSet(&TestClick);
	//////menu2.AddSeparator();
	//////menu2.AddRadioItem("Radio1", true).OnClickSet(&TestClick);
	//////menu2.AddRadioItem("Radio2", true).OnClickSet(&TestClick);
	//////menu2.AddRadioItem("Radio3", false).OnClickSet(&TestClick);
	//////menu2.AddRadioItem("Radio4", true).OnClickSet(&TestClick);
	//////menu2.AddSeparator();
	//////menu2.AddRadioItem("Radio1", false).OnClickSet(&TestClick);
	//////menu2.AddRadioItem("Radio2", true).OnClickSet(&TestClick);
	//////menu1.AddItem("Submenu-Item4").OnClickSet(&TestClick);
	//////menu1.AddItem("Submenu-Item5").OnClickSet(&TestClick);
	//////window->UpdateMenuStrip();
	////
	//////item1.OnClickSet(&TestClick);
	//////item2.OnClickSet(&TestClick);
	//////menu1.OnClickSet(&TestClick);
	//////menu2.OnClickSet(&TestClick);
	////
    button1 = window->AddButton("Abc", 200, 80, 50, 100);
    button1->OnClickSet(&TestClick);
    button1->OnMouseEnterSet([](Object* sender, EventArgs* e)
    	{
    		printf_s("Teste Mouseenter Button1");
    	}
    );
    button1->OnMouseLeaveSet([](Object* sender, EventArgs* e)
    	{
    		printf_s("Teste Mouseleave Button1");
    	}
    );
    
    button2 = window->AddButton("XYZ", 100, 40, 200, 200);
    button2->OnClickSet([](Object* sender, EventArgs* e)
    	{
    		Button* b = (Button*)sender;
            printf_s("Button2 clicked");
    	}
    );
    
    txtBox1 = window->AddTextBox("TextBox", 400, 20, 400);
    txtBox1->BorderStyle = BorderStyle::None;
    txtBox1->OnKeyPressSet([](Object* sender, KeyPressEventArgs* e)
    	{
    		TextBox* t = (TextBox*)sender;
    		if (e->KeyChar == 'p')
    		{
                printf_s("printing P");
    		}
    		//if (e->KeyChar == 'q')
    		//{
    	//		e->Handled = true;
    	//		return;
    		//}
    	}
    );
    
    txtBox2 = window->AddTextBox("TextBox2", 400, 20, 460);
    txtBox2->BorderStyle = BorderStyle::FixedSingle;
    txtBox2->OnKeyPressSet([](Object* sender, KeyPressEventArgs* e)
        {
            TextBox* t = (TextBox*)sender;
            if (e->KeyChar == 'z')
            {
                printf_s("printing Z");
            }
        });
    
    
    txtBox3 = window->AddTextBox("TextBox3", 400, 20, 560);
    txtBox3->BorderStyle = BorderStyle::Fixed3D;
    txtBox3->OnKeyPressSet([](Object* sender, KeyPressEventArgs* e)
    	{
    		TextBox* t = (TextBox*)sender;
    		if (e->KeyChar == 'A')
    		{
                printf_s("printing A");
    		}
    		//if (e->KeyChar == 'q')
    		//{
    	//		e->Handled = true;
    	//		return;
    		//}
    	}
    );
    
    progressBar = window->AddProgressBar("abc", 300, 50, 50, 20);
}