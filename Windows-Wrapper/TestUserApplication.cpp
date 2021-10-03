#include "TestUserApplication.h"

void TestClick(Object* sender, EventArgs* e)
{
	std::ostringstream ossin;
	ossin << sender->ToString() << std::endl << e->ToString() << std::endl << "Clicked!" << std::endl << std::endl;
	printf_s(ossin.str().c_str());
}

void TestUserApplication::Initialize()
{   
	Window* window = new Window("Teste", 1000, 1000);
	window->GetMouse().EnableRaw();
	window->GetKeyboard().DisableAutorepeat();
	
	auto lb = window->AddListBox(260, 240, 450, 50);
    lb->SetSelectionMode(SelectionMode::Single);
    std::vector<ListItem> list;
	for (int i = 0; i < 1000; ++i)
	{
		std::ostringstream oss;
		oss << "Item: " << i;
		list.push_back(ListItem(i, oss.str()));
	}
	lb->SetDataSource(list);
	lb->SetBorderStyle(BorderStyle::Fixed3D);
	lb->EnableMultiColumn();
	//lb->SetColumnWidth(180);
	
	auto cb = window->AddComboBox("Start Text", 260, 450, 450);

    std::vector<ListItem> list2;
	for (int i = 0; i < 5; ++i)
	{
		std::ostringstream oss;
		oss << "Item: " << i;
		list2.push_back(ListItem(i, oss.str()));
	}
	cb->SetDataSource(list2);
    
    button1 = window->AddButton("Abc", 200, 80, 50, 100);
    button1->OnClickSet(&TestClick);
    button1->OnMouseEnterSet([](Object* sender, EventArgs* e)
    	{
			std::ostringstream ossin;
			ossin << sender->ToString() << std::endl << e->ToString() << std::endl << std::endl;
			printf_s(ossin.str().c_str());
    		printf_s("Teste Mouseenter Button1");
    	}
    );
    button1->OnMouseLeaveSet([](Object* sender, EventArgs* e)
    	{
			std::ostringstream ossin;
			ossin << sender->ToString() << std::endl << e->ToString() << std::endl << std::endl;
			printf_s(ossin.str().c_str());
    		printf_s("Teste Mouseleave Button1");
    	}
    );
    
    button2 = window->AddButton("XYZ", 100, 40, 200, 200);
    button2->OnClickSet([](Object* sender, EventArgs* e)
    	{
			std::ostringstream ossin;
			ossin << sender->ToString() << std::endl << e->ToString() << std::endl << std::endl;
			printf_s(ossin.str().c_str());
            printf_s("Button2 clicked");
    	}
    );
    
    txtBox1 = window->AddTextBox("TextBox", 400, 20, 400);
    txtBox1->BorderStyle = BorderStyle::None;
    txtBox1->OnKeyPressSet([](Object* sender, KeyPressEventArgs* e)
    	{
			std::ostringstream ossin;
			ossin << sender->ToString() << std::endl << e->ToString() << std::endl << std::endl;
			printf_s(ossin.str().c_str());
    		if (e->KeyChar == 'p')
    		{
                printf_s("printing P");
    		}
    	}
    );
    
    txtBox2 = window->AddTextBox("TextBox2", 400, 20, 460);
    txtBox2->BorderStyle = BorderStyle::FixedSingle;
    txtBox2->OnKeyPressSet([](Object* sender, KeyPressEventArgs* e)
        {
			std::ostringstream ossin;
			ossin << sender->ToString() << std::endl << e->ToString() << std::endl << std::endl;
			printf_s(ossin.str().c_str());
            if (e->KeyChar == 'z')
            {
                printf_s("printing Z");
            }
        });
    
    
    txtBox3 = window->AddTextBox("TextBox3", 400, 20, 560);
    txtBox3->BorderStyle = BorderStyle::Fixed3D;
    txtBox3->OnKeyPressSet([](Object* sender, KeyPressEventArgs* e)
    	{
			std::ostringstream ossin;
			ossin << sender->ToString() << std::endl << e->ToString() << std::endl << std::endl;
			printf_s(ossin.str().c_str());
    		if (e->KeyChar == 'A')
    		{
                printf_s("printing A");
    		}
    	}
    );
    
    progressBar = window->AddProgressBar("abc", 300, 50, 50, 20);
}