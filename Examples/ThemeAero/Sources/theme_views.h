
#pragma once

namespace clan
{
	class ThemeButtonView : public ButtonView
	{
	public:
		ThemeButtonView();
	};

	class ThemeCheckBoxView : public CheckBoxView
	{
	public:
		ThemeCheckBoxView();
	};

	class ThemeImageView : public ImageView
	{
	public:
		ThemeImageView();
	};

	class ThemeLabelView : public LabelView
	{
	public:
		ThemeLabelView();
	};

	class ThemeListBoxView : public ListBoxView
	{
	public:
		ThemeListBoxView();
	};

	class ThemeListBoxLabelView : public LabelView
	{
	public:
		ThemeListBoxLabelView(const std::string &text = std::string());
	};

	class ThemeRadioButtonView : public RadioButtonView
	{
	public:
		ThemeRadioButtonView();
	};

	class ThemeScrollView : public ScrollView
	{
	public:
		ThemeScrollView();
	};

	class ThemeScrollBarView : public ScrollBarView
	{
	public:
		ThemeScrollBarView();
	};

	class ThemeSliderView : public SliderView
	{
	public:
		ThemeSliderView();
	};

	class ThemeSpinView : public SpinView
	{
	public:
		ThemeSpinView();
	};

	class ThemeTextFieldView : public TextFieldView
	{
	public:
		ThemeTextFieldView();
	};

	class ThemeTextView : public TextView
	{
	public:
		ThemeTextView();
	};
}
