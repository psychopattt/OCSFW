#include "WindowTitle.h"

WindowTitle::WindowTitle() : WindowTitle("") { }

WindowTitle::WindowTitle(string title) : WindowTitle(title.c_str()) { }

WindowTitle::WindowTitle(const char* title)
{
	text = title;
}

void WindowTitle::SetText(string text)
{
	modified = true;
	this->text = text;
}

WindowTitle* WindowTitle::SetSubText(string text)
{
	WindowTitle* subTitle = GetSubTitle();
	subTitle->SetText(text);
	return subTitle;
}

WindowTitle* WindowTitle::GetSubTitle()
{
	if (!subTitle)
		subTitle = std::make_unique<WindowTitle>();

	return subTitle.get();
}

string WindowTitle::ToString() const
{
	return text + (subTitle ? " - " + subTitle->ToString() : "");
}

bool WindowTitle::IsOutdated() const
{
	return modified || (subTitle && subTitle->IsOutdated());
}

void WindowTitle::Update()
{
	if (subTitle)
		subTitle->Update();

	modified = false;
}
