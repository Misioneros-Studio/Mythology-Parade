#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Fonts.h"
#include "j1Input.h"
#include "j1Window.h"
#include "j1Gui.h"
#include "Console.h"
#include "j1Audio.h"

j1Gui::j1Gui() : j1Module()
{
	name.create("gui");
}

// Destructor
j1Gui::~j1Gui()
{}

// Called before render is available
bool j1Gui::Awake(pugi::xml_node& conf)
{
	LOG("Loading GUI atlas");
	bool ret = true;

	atlas_file_name = conf.child("atlas").attribute("file").as_string("");

	return ret;
}

// Called before the first frame
bool j1Gui::Start()
{
	atlas = App->tex->Load(atlas_file_name.GetString());
	click_sfx = App->audio->LoadFx("audio/fx/button_click.wav");

	return true;
}

// Update all guis
bool j1Gui::PreUpdate()
{
	bool mouse = false;
	lockClick = false;
	int count = 0;
	if (App->input->GetMouseButtonDown(1) == KEY_DOWN || App->input->GetMouseButtonDown(1) == KEY_REPEAT) {
		for (int i = UIs.count() - 1; i >= 0 && mouse == false; i--) {
			mouse = UIs.At(i)->data->CheckMouse();
			if (mouse == true)count = i;
		}
	}
	if (mouse == true) {
		UIs.At(count)->data->Move();
	}
	for (int i = 0; i < UIs.count(); i++)
	{
		UIs.At(i)->data->PreUpdate();
	}
	return true;
}

// Called after all Updates
bool j1Gui::PostUpdate()
{
	for (int i = 0; i < UIs.count(); i++) {
		UIs.At(i)->data->PostUpdate();
	}
	return true;
}

// Called before quitting
bool j1Gui::CleanUp()
{
	LOG("Freeing GUI");
	for (int i = UIs.count() - 1; i >= 0; i--)
	{
		UIs.At(i)->data->CleanUp();
		UIs.del(UIs.At(i));
	}
	UIs.clear();
	if (atlas)
	{
		App->tex->UnLoad(atlas);
	}
	return true;
}

// const getter for atlas
const SDL_Texture* j1Gui::GetAtlas() const
{
	return atlas;
}

// class Gui ---------------------------------------------------

UI* j1Gui::CreateUIElement(Type type, UI* p, SDL_Rect r, SDL_Rect sprite, p2SString str, SDL_Rect sprite2, SDL_Rect sprite3, bool drageable, SDL_Rect drag_area, j1Module* s_listener,
	bool console, float drag_position_scroll_bar)
{
	UI* ui = nullptr;
	SDL_Color colour;
	bool title;
	switch (type)
	{
	case Type::BUTTON:
		ui = new ButtonUI(Type::BUTTON, p, r, sprite, sprite2, sprite3, true, true, drag_area);
		break;
	case Type::IMAGE:
		ui = new ImageUI(Type::IMAGE, p, r, sprite, drageable, drageable, drag_area, drag_position_scroll_bar);
		break;
	case Type::WINDOW:
		ui = new WindowUI(Type::WINDOW, p, r, sprite, drageable, drageable, drag_area);
		break;
	case Type::TEXT:
		colour = { (Uint8)sprite2.x,(Uint8)sprite2.y,(Uint8)sprite2.w,(Uint8)sprite2.h };
		if (sprite3.x == 0)
			title = false;
		else
			title = true;
		ui = new TextUI(Type::TEXT, p, r, str, drageable, drageable, drag_area, console, colour, title);
		break;
	case Type::LISTTEXTS:
		ui = new ListTextsUI(Type::LISTTEXTS, p, r, str, drageable, drageable, drag_area, console);
		break;
	}

	ui->active = true;
	ui->name = str;

	if (s_listener)
	{
		ui->listener = s_listener;
	}
	else
	{
		ui->listener = nullptr;
	}


	//UIs.add(ui);

	return UIs.add(ui)->data;
}

UI* j1Gui::CreateUIElement(Type type, UI* p, SDL_Rect r, p2SString str, int re, int g, int b, int a, bool drageable, SDL_Rect drag_area, j1Module* s_listener)
{
	UI* ui = nullptr;
	switch (type)
	{
	case Type::IMAGE:
		ui = new ImageUI(Type::IMAGE, p, r, re, g, b, a, drageable, drageable, drag_area);
		break;
	case Type::INPUT:
		ui = new TextInputUI(Type::INPUT, p, r, re, g, b, a, "", drageable, true, drag_area);
		break;
	}

	ui->active = true;
	ui->name = str;

	if (s_listener)
	{
		ui->listener = s_listener;
	}
	else
	{
		ui->listener = nullptr;
	}


	//UIs.add(ui);

	return UIs.add(ui)->data;
}

bool j1Gui::DeleteUIElement(UI* ui) {
	int n = UIs.find(ui);
	if (n == -1)return false;
	else
	{
		UIs.At(n)->data->CleanUp();
		UIs.del(UIs.At(n));
		return true;
	}
}

void j1Gui::ChangeDebug() {
	for (int i = 0; i < UIs.count(); i++) {
		UIs.At(i)->data->debug = !UIs.At(i)->data->debug;
	}
}

void j1Gui::ChangeFocus() {
	bool exit = false;
	bool focus = false;
	int count = 0;
	for (int i = 0; i < UIs.count() && exit == false; i++) {
		bool focusable = UIs.At(i)->data->CheckFocusable();
		if (focusable == true) {
			count++;
			if (focus == true) {
				UIs.At(i)->data->focus = true;
				exit = true;
			}
			else {
				focus = UIs.At(i)->data->focus;
				UIs.At(i)->data->focus = false;
			}
		}
	}
	if (count > 0 && exit == false) {
		for (int i = 0; i < UIs.count() && exit == false; i++) {
			bool focusable = UIs.At(i)->data->CheckFocusable();
			if (focusable == true) {
				UIs.At(i)->data->focus = true;
				exit = true;
			}
		}
	}
}

void j1Gui::DeleteFocus() {
	for (int i = 0; i < UIs.count(); i++) {
		UIs.At(i)->data->focus = false;
	}
}

void j1Gui::ClearUI()
{
	UIs.clear();
}

void j1Gui::ReturnConsole() {
	if (App->console->console_active == true) {
		App->console->ActivateConsole();
	}
}

void j1Gui::WorkWithTextInput(p2SString text) {
	bool exit = false;
	for (int i = 0; i < UIs.count() && exit == false; i++) {
		if (UIs.At(i)->data->type == Type::INPUT && UIs.At(i)->data->focus == true) {
			TextInputUI* text_ui = (TextInputUI*)UIs.At(i)->data;
			text_ui->ChangeLabel(text);
		}
	}
}

UI::UI(Type s_type, SDL_Rect r, UI* p, bool d, bool f, SDL_Rect d_area, bool consol)
{
	name.create("UI");
	type = s_type;
	drageable = d;
	focusable = f;
	screen_rect = { r.x,r.y,r.w,r.h };
	parent = p;
	if (parent != nullptr) {
		local_rect = { screen_rect.x - parent->screen_rect.x, screen_rect.y - parent->screen_rect.y, screen_rect.w, screen_rect.h };
	}
	else {
		local_rect = screen_rect;
	}
	mask_rect = screen_rect;
	debug = false;
	focus = false;
	drag_area = d_area;
	console = consol;
}

bool UI::PreUpdate() {
	UI* ui = this;

	screen_rect.x = local_rect.x;
	screen_rect.y = local_rect.y;
	while (ui->parent != nullptr) {
		screen_rect.x += ui->parent->local_rect.x;
		screen_rect.y += ui->parent->local_rect.y;
		ui = ui->parent;
	}

	uint win_x, win_y;
	App->win->GetWindowSize(win_x, win_y);
	mask_rect = screen_rect;

	if (parent != nullptr) {
		if (mask_rect.x < parent->mask_rect.x) {
			mask_rect.x += parent->mask_rect.x - mask_rect.x;
			mask_rect.w -= parent->mask_rect.x - mask_rect.x;
		}
		else if (mask_rect.x + mask_rect.w > parent->mask_rect.x + parent->mask_rect.w) {
			mask_rect.w -= mask_rect.x + mask_rect.w - parent->mask_rect.x - parent->mask_rect.w;
		}
		if (mask_rect.y < parent->mask_rect.y) {
			mask_rect.y += parent->mask_rect.y - mask_rect.y;
			mask_rect.h -= parent->mask_rect.y - mask_rect.y;
		}
		else if (mask_rect.y + mask_rect.h > parent->mask_rect.y + parent->mask_rect.h) {
			mask_rect.h -= mask_rect.y + mask_rect.h - parent->mask_rect.y - parent->mask_rect.h;
		}
	}
	else {
		if (mask_rect.x < 0) {
			mask_rect.w -= mask_rect.x;
			mask_rect.x = 0;
		}
		else if (mask_rect.x + mask_rect.w > win_x) {
			mask_rect.w -= mask_rect.x + mask_rect.w - win_x;
		}
		if (mask_rect.y < 0) {
			mask_rect.h -= mask_rect.y;
			mask_rect.y = 0;
		}
		else if (mask_rect.y + mask_rect.h > win_y) {
			mask_rect.h -= mask_rect.y + mask_rect.h - win_y;
		}
	}
	return true;
}

bool UI::PostUpdate() {
	if (debug == true) {
		App->render->DrawQuad(screen_rect, 255, 0, 0, 255, false, false);
	}
	return true;
}

SDL_Rect UI::GetScreenRect()
{
	return screen_rect;
}
SDL_Rect UI::GetParentScreenRect()
{
	if (parent != nullptr)
		return parent->screen_rect;
	else {
		uint width, height;
		App->win->GetWindowSize(width, height);
		return { 0,0,(int)width,(int)height };
	}
}
SDL_Rect UI::GetLocalRect() {
	return local_rect;
}
iPoint UI::GetScreenPos() {
	return { screen_rect.x,screen_rect.y };
}
iPoint UI::GetScreenToWorldPos() {
	uint scale = App->win->GetScale();
	return { screen_rect.x / (int)scale,screen_rect.y / (int)scale };
}
iPoint UI::GetLocalPos() {
	return { local_rect.x,local_rect.y };
}
void UI::SetLocalPos(iPoint pos) {
	iPoint r = { -local_rect.x + pos.x,-local_rect.y + pos.y };
	local_rect.x = pos.x;
	local_rect.y = pos.y;
	screen_rect.x += r.x;
	screen_rect.y += r.y;
}

void UI::SetScreenRect(SDL_Rect rect) {
	screen_rect = rect;
}

void UI::UpdateLocalRect() {
	if (parent != nullptr) {
		local_rect = { screen_rect.x - parent->screen_rect.x, screen_rect.y - parent->screen_rect.y, screen_rect.w, screen_rect.h };
	}
	else {
		local_rect = screen_rect;
	}
}

bool UI::CheckMouse() {
	if (drageable == true) {
		int x, y;
		App->input->GetMousePosition(x, y);
		if ((x >= screen_rect.x && x <= screen_rect.x + screen_rect.w && y >= screen_rect.y && y <= screen_rect.y + screen_rect.h) || focus == true)
			return true;
	}
	return false;
}

bool UI::Move() {
	int x, y;
	App->input->GetMouseMotion(x, y);
	x *= App->win->GetScale();
	y *= App->win->GetScale();
	if (screen_rect.x + x >= drag_area.x && screen_rect.x + screen_rect.w + x <= drag_area.x + drag_area.w) {
		local_rect.x += x;
		quad.x += x;
	}
	else if (screen_rect.y + y >= drag_area.y && screen_rect.y + screen_rect.h + y <= drag_area.y + drag_area.h) {
		local_rect.y += y;
		quad.y += y;
	}
	return true;
}

SDL_Rect UI::Check_Printable_Rect(SDL_Rect sprite, iPoint& dif_sprite) {
	if (mask_rect.x > screen_rect.x) {
		dif_sprite.x = mask_rect.x - screen_rect.x;
		sprite.x += dif_sprite.x;
		sprite.w -= dif_sprite.x;
	}
	else if (mask_rect.w < screen_rect.w) {
		sprite.w -= screen_rect.w - mask_rect.w;
	}
	if (mask_rect.y > screen_rect.y) {
		dif_sprite.y = mask_rect.y - screen_rect.y;
		sprite.y += dif_sprite.y;
		sprite.h -= dif_sprite.y;
	}
	else if (mask_rect.h < screen_rect.h) {
		sprite.h -= screen_rect.h - mask_rect.h;
	}
	return sprite;
}

SDL_Rect UI::Check_Printable_Rect(SDL_Rect sprite, iPoint& dif_sprite, SDL_Rect quad) {
	SDL_Rect parent_screen_rect = GetParentScreenRect();
	if (mask_rect.x > quad.x) {
		dif_sprite.x = mask_rect.x - quad.x;
		sprite.x += dif_sprite.x;
		sprite.w -= dif_sprite.x;
	}
	else if (mask_rect.w < quad.w) {
		sprite.w -= quad.w - mask_rect.w;
	}
	if (parent_screen_rect.y > quad.y) {
		dif_sprite.y = parent_screen_rect.y - quad.y;
		sprite.y += dif_sprite.y;
		sprite.h -= dif_sprite.y;
	}
	else if (parent_screen_rect.y + parent_screen_rect.h < quad.y + quad.h) {
		sprite.h -= quad.y + quad.h - parent_screen_rect.y - parent_screen_rect.h;
	}
	return sprite;
}

ImageUI::ImageUI(Type type, UI* p, SDL_Rect r, SDL_Rect sprite, bool d, bool f, SDL_Rect d_area, float drag_position_scroll_bar) :UI(type, r, p, d, f, d_area) {
	name.create("ImageUI");
	sprite1 = sprite;
	quad = r;
	SDL_Rect drag_area = GetDragArea();
	drag_position_0 = { drag_area.x, drag_area.y };
	drag_position_1 = { drag_area.w + drag_area.x - GetLocalRect().w,drag_area.h + drag_area.y - GetLocalRect().h };
	square = false;
	red = green = blue = alpha = 0;
	if (drag_position_scroll_bar != -1) {
		quad.x = drag_position_0.x + (drag_position_scroll_bar * (drag_position_1.x - drag_position_0.x));
		SetScreenRect(quad);
		UpdateLocalRect();
	}
}

ImageUI::ImageUI(Type type, UI* p, SDL_Rect r, int re, int g, int b, int a, bool d, bool f, SDL_Rect d_area) :UI(type, r, p, d, f, d_area, true) {
	name.create("ImageUI");
	sprite1 = { 0,0,0,0 };
	quad = r;
	SDL_Rect drag_area = GetDragArea();
	drag_position_0 = { drag_area.x, drag_area.y };
	drag_position_1 = { drag_area.w + drag_area.x - GetLocalRect().w,drag_area.h + drag_area.y - GetLocalRect().h };
	square = true;
	red = re;
	green = g;
	blue = b;
	alpha = a;
}

bool ImageUI::PreUpdate() {
	int x, y;
	iPoint initial_position = GetScreenPos();
	App->input->GetMousePosition(x, y);
	if (CheckFocusable() == true && (x >= GetScreenPos().x && x <= GetScreenPos().x + GetScreenRect().w && y >= GetScreenPos().y && y <= GetScreenPos().y + GetScreenRect().h)) {
		if (App->input->GetMouseButtonDown(1) == KEY_DOWN) {
			App->gui->DeleteFocus();
			focus = true;
		}
	}
	if (focus == true && App->input->GetMouseButtonDown(1) == KEY_UP) {
		focus = false;
	}
	UI::PreUpdate();
	if (initial_position != GetScreenPos()) {
		fPoint drag_position = GetDragPositionNormalized();
		if (listener)
		{
			listener->OnClick(this, drag_position.x);
		}
	}
	return true;
}

bool ImageUI::PostUpdate() {
	iPoint dif_sprite = { 0,0 };
	if (square == false) {
		SDL_Rect sprite = UI::Check_Printable_Rect(sprite1, dif_sprite);
		quad.x = GetScreenPos().x + dif_sprite.x;
		quad.y = GetScreenPos().y + dif_sprite.y;
		if (this->active) App->render->BlitInsideQuad((SDL_Texture*)App->gui->GetAtlas(), sprite, quad);
	}
	else if (this->active) {
		App->render->DrawQuad(quad, red, green, blue, alpha, true, false);
	}
	UI::PostUpdate();
	return true;
}

fPoint ImageUI::GetDragPositionNormalized() {
	fPoint position_normalized;
	position_normalized.x = GetScreenPos().x - drag_position_0.x;
	position_normalized.y = GetScreenPos().y - drag_position_0.y;
	position_normalized.x /= drag_position_1.x - drag_position_0.x;
	position_normalized.y /= drag_position_1.y - drag_position_0.y;
	return position_normalized;
}

WindowUI::WindowUI(Type type, UI* p, SDL_Rect r, SDL_Rect sprite, bool d, bool f, SDL_Rect d_area) :UI(type, r, p, d, f, d_area) {
	name.create("WindowUI");
	sprite1 = sprite;
	quad = r;
}

bool WindowUI::PostUpdate() {
	iPoint dif_sprite = { 0,0 };
	SDL_Rect sprite = UI::Check_Printable_Rect(sprite1, dif_sprite);
	quad.x = GetScreenPos().x + dif_sprite.x;
	quad.y = GetScreenPos().y + dif_sprite.y;
	if (this->active)App->render->BlitInsideQuad((SDL_Texture*)App->gui->GetAtlas(), sprite, quad);

	UI::PostUpdate();
	return true;
}

TextUI::TextUI(Type type, UI* p, SDL_Rect r, p2SString str, bool d, bool f, SDL_Rect d_area, bool console, SDL_Color coulor, bool title) :UI(type, r, p, d, f, d_area, console) {
	name.create("TextUI");
	stri = str;
	quad = r;
	color = coulor;
	title_default = title;
}

bool TextUI::PostUpdate() {
	SDL_Rect rect = { 0,0,0,0 };
	iPoint dif_sprite = { 0,0 };

	SDL_Texture* text;
	if(title_default==false)
		text = App->font->Print(stri.GetString(), color);
	else
		text = App->font->Print(stri.GetString(), color, App->font->default_title);

	SDL_QueryTexture(text, NULL, NULL, &rect.w, &rect.h);


	SDL_Rect sprite = UI::Check_Printable_Rect(rect, dif_sprite);
	if (this->active && this->GetConsole() == false)App->render->Blit(text, GetScreenToWorldPos().x + dif_sprite.x, GetScreenToWorldPos().y + dif_sprite.y, &sprite, 0.0F);
	else if (this->active) App->render->Blit(text, quad.x + dif_sprite.x, quad.y + dif_sprite.y, &sprite, 0.0F);
	UI::PostUpdate();

	App->tex->UnLoad(text);

	return true;
}

void TextUI::SetString(p2SString new_string) {
	stri = new_string;
}

ListTextsUI::ListTextsUI(Type type, UI* p, SDL_Rect r, p2SString str, bool d, bool f, SDL_Rect d_area, bool console) :UI(type, r, p, d, f, d_area, console) {
	name.create("ListTextsUI");
	stri.add(str);
	number_of_stri = stri.count();
	quad = r;
}



bool ListTextsUI::PostUpdate() {

	SDL_Rect rect = { 0,0,0,0 };
	iPoint dif_sprite = { 0,0 };

	for (int i = 0; i < number_of_stri; i++) {
		dif_sprite = { 0,0 };

		SDL_Texture* text = App->font->Print(stri.At(i)->data.GetString());

		SDL_QueryTexture(text, NULL, NULL, &rect.w, &rect.h);


		SDL_Rect sprite = UI::Check_Printable_Rect(rect, dif_sprite, { quad.x,quad.y + (quad.h * i),quad.w,quad.h });
		if (this->active && this->GetConsole() == false)App->render->Blit(text, GetScreenToWorldPos().x + dif_sprite.x, GetScreenToWorldPos().y + dif_sprite.y, &sprite, 0.0F);
		else if (this->active) App->render->Blit(text, quad.x + dif_sprite.x, quad.y + dif_sprite.y + (i * quad.h), &sprite, 0.0F);

		App->tex->UnLoad(text);
	}
	UI::PostUpdate();

	return true;
}

void ListTextsUI::SetListOfStrings(p2SString string, int position) {
	if (position > number_of_stri) {
		stri.add(string);
		number_of_stri++;
		SDL_Rect screen_rect = GetScreenRect();
		SDL_Rect parent_screen_rect = GetParentScreenRect();
		screen_rect.h += quad.h;
		quad.y = screen_rect.y = (parent_screen_rect.y + parent_screen_rect.h) - screen_rect.h;
		SetScreenRect(screen_rect);
		UpdateLocalRect();
	}
}

ButtonUI::ButtonUI(Type type, UI* p, SDL_Rect r, SDL_Rect sprite, SDL_Rect spriten2, SDL_Rect spriten3, bool d, bool f, SDL_Rect d_area) :UI(type, r, p, d, f, d_area) {
	name.create("ButtonUI");
	sprite1 = sprite;
	sprite2 = spriten2;
	sprite3 = spriten3;
	over = false;
	pushed = false;
	quad = r;
	isLocked = false;
	front = true;
}

bool ButtonUI::PostUpdate() {
	SDL_Rect sprite;
	iPoint dif_sprite = { 0,0 };
	if (pushed == true || isLocked) {
		sprite = UI::Check_Printable_Rect(sprite2, dif_sprite);
	}
	else if (over == true) {
		sprite = UI::Check_Printable_Rect(sprite1, dif_sprite);
	}
	else {
		sprite = UI::Check_Printable_Rect(sprite3, dif_sprite);
	}
	//App->render->Blit((SDL_Texture*)App->gui->GetAtlas(), GetScreenToWorldPos().x + dif_sprite.x, GetScreenToWorldPos().y + dif_sprite.y, &sprite, 0.f);

	quad.x = GetScreenPos().x + dif_sprite.x;
	quad.y = GetScreenPos().y + dif_sprite.y;
	if (this->active)App->render->BlitInsideQuad((SDL_Texture*)App->gui->GetAtlas(), sprite, quad);

	UI::PostUpdate();
	return true;
}

bool ButtonUI::PreUpdate() {
	int x, y;
	App->input->GetMousePosition(x, y);

	if (front == true && ((x >= GetScreenPos().x && x <= GetScreenPos().x + GetScreenRect().w && y >= GetScreenPos().y && y <= GetScreenPos().y + GetScreenRect().h) || focus == true))
		over = true;
	else over = false;
	bool button = false;
	if (App->input->GetMouseButtonDown(1) == KEY_UP || App->input->GetKey(SDL_SCANCODE_RETURN))
		button = true;
	if (over == true && button == true)
		pushed = true;
	else pushed = false;
	if (pushed && !App->gui->lockClick && !isLocked)
	{
		App->audio->PlayFx(App->gui->click_sfx);
		//Button clicked
		if (listener)
		{
			listener->OnClick(this);
		}
		App->gui->lockClick = true;
		LOG("Click");
	}

	UI::PreUpdate();

	return true;
}


TextInputUI::TextInputUI(Type type, UI* p, SDL_Rect r, int re, int g, int b, int a, p2SString str, bool d, bool f, SDL_Rect d_area) :UI(type, r, p, d, f, d_area, true) {
	name.create("TextInputUI");
	sprite1 = { 0,0,0,0 };
	quad = r;
	label = str;
	text_input = false;
	position = 0;
	square = true;
	red = re;
	green = g;
	blue = b;
	alpha = a;
}

bool TextInputUI::PreUpdate() {
	int x, y;
	App->input->GetMousePosition(x, y);
	if ((x >= GetScreenPos().x && x <= GetScreenPos().x + GetScreenRect().w && y >= GetScreenPos().y && y <= GetScreenPos().y + GetScreenRect().h)) {
		if (App->input->GetMouseButtonDown(1) == KEY_DOWN) {
			App->gui->DeleteFocus();
			focus = true;
		}
	}
	if (focus == true && text_input == false) {
		SDL_StartTextInput();
		text_input = true;
	}
	else if (focus == false && text_input == true) {
		SDL_StopTextInput();
		text_input = false;
	}
	if (focus == true) {
		if (App->input->special_keys == specialkeys::Backspace) {
			label = label.Backspace(position);
			if (position > 0)
				position--;
		}
		else if (App->input->special_keys == specialkeys::Left) {
			if (position > 0)
				position--;
		}
		else if (App->input->special_keys == specialkeys::Right) {
			if (position < label.GetCapacity())
				position++;
		}
		else if (App->input->special_keys == specialkeys::Supr) {
			label = label.Supr(position);
		}
		else if (App->input->special_keys == specialkeys::Home) {
			position = 0;
		}
		else if (App->input->special_keys == specialkeys::End) {
			position = label.GetCapacity();
		}

	}
	UI::PreUpdate();
	return true;
}

bool TextInputUI::PostUpdate() {
	iPoint dif_sprite = { 0,0 };
	if (this->active) {
		App->render->DrawQuad(quad, red, green, blue, alpha, true, false);
	}

	SDL_Rect rect = { 0,0,0,0 };
	if (strcmp(label.GetString(), "")) {
		SDL_Texture* text = App->font->Print(label.GetString());
		SDL_QueryTexture(text, NULL, NULL, &rect.w, &rect.h);
		SDL_Rect sprite = UI::Check_Printable_Rect(rect, dif_sprite);
		if (this->active) App->render->Blit(text, quad.x + dif_sprite.x, quad.y + dif_sprite.y, &sprite, 0.0F);
		App->tex->UnLoad(text);
	}

	if (focus == true) {
		p2SString label_from_position = label.StrFromPosition(position);
		int w, h;
		App->font->CalcSize(label_from_position.GetString(), w, h);
		App->render->DrawLine(quad.x + w, quad.y, quad.x + w, quad.y + quad.h, 0, 255, 255, 255, false);
	}
	UI::PostUpdate();



	return true;
}

void TextInputUI::ChangeLabel(p2SString text) {
	p2SString label_part_2, label_part_1;
	label_part_1 = label_part_2 = label;
	for (int i = position; i < label.Length(); i++) {
		label_part_1 = label_part_1.Supr(position);
	}
	if (strcmp(label.GetString(), label_part_1.GetString())) {
		for (int i = 0; i < position; i++) {
			label_part_2 = label_part_2.Supr(0);
		}
	}
	else {
		label_part_2 = "";
	}
	label_part_1 += text.GetString();
	label_part_1 += label_part_2;
	label = label_part_1;
	position++;
}

void TextInputUI::SetLabel(p2SString text) {
	if (App->input->GetKey(SDL_SCANCODE_GRAVE) != KEY_DOWN) {
		label = text.GetString();
		position += label.GetCapacity();
	}
}

void TextInputUI::SetPositionToZero() {
	position = 0;
}