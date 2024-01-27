#pragma once

#include <string>
#include <memory>

using std::string;

class WindowTitle
{
	public:
		WindowTitle();
		WindowTitle(string title);
		WindowTitle(const char* title);
		void SetText(string text);
		WindowTitle* SetSubText(string text);
		WindowTitle* GetSubTitle();
		string ToString() const;
		bool IsOutdated() const;
		void Update();

	private:
		string text;
		bool modified = false;
		std::unique_ptr<WindowTitle> subTitle;
};
