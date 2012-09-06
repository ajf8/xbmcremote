/*
 * xbmcremote
 * Copyright (C) 2012  Alan Fitton

 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include <gtkmm.h>
#include <glibmm.h>

#include "util.h"
#include "preferences-dialog.h"

namespace XbmcRemote {

void PreferencesDialog::on_bound_sensitive_active_toggled(Gtk::CheckButton *check, Gtk::Widget *widget)
{
  widget->set_sensitive(check->get_active());
}

void PreferencesDialog::bind_active_sensitive(Gtk::CheckButton *check, Gtk::Widget *widget)
{
  on_bound_sensitive_active_toggled(check, widget);
  check->signal_toggled().connect(sigc::bind(sigc::mem_fun(*this, &PreferencesDialog::on_bound_sensitive_active_toggled), check, widget));
}

Gtk::Widget* PreferencesDialog::construct_connection_page()
{
  Glib::RefPtr<Gtk::Builder> builder = Gtk::Builder::create_from_file(
      Util::build_glade_path("preferences-connection.glade"));
  Gtk::Entry *hostnameEntry;
  Gtk::Entry *userEntry;
  Gtk::Entry *passEntry;
  Gtk::SpinButton *webPortSpin;
  Gtk::SpinButton *jsonPortSpin;
  Gtk::CheckButton *authCheck;

  builder->get_widget("connection-vbox", m_currentPage);
  builder->get_widget("json_port_spin", jsonPortSpin);
  builder->get_widget("web_port_spin", webPortSpin);
  builder->get_widget("hostname_entry", hostnameEntry);
  builder->get_widget("ck_auth", authCheck);
  builder->get_widget("entry_user", userEntry);
  builder->get_widget("entry_pass", passEntry);

  m_refSettings->bind("hostname", hostnameEntry->property_text());
  m_refSettings->bind("auth-enabled", authCheck->property_active());
  m_refSettings->bind("username", userEntry->property_text());
  m_refSettings->bind("password", passEntry->property_text());
  bind_active_sensitive(authCheck, userEntry);
  bind_active_sensitive(authCheck, passEntry);
  m_refSettings->bind("web-port", webPortSpin->property_value());
  m_refSettings->bind("rpc-port", jsonPortSpin->property_value());

  m_pluginContainer->add(*m_currentPage);

  return m_currentPage;
}

void PreferencesDialog::page_selection_changed()
{
  Gtk::TreeModel::Row row = *(m_pluginTree->get_selection()->get_selected());

  m_pluginLabel->set_markup(Glib::ustring::format("<span size=\"xx-large\"><b>", row[m_pageStoreCols.m_col_name], "</b></span>"));

  if (m_currentPage) {
    delete m_currentPage;
    m_currentPage = NULL;
  }

  switch (row[m_pageStoreCols.m_col_id])
  {
  case PP_CONNECTION:
    construct_connection_page();
    break;
  case PP_DESKTOP:
    break;
  }
}

bool PreferencesDialog::header_draw(const Cairo::RefPtr<Cairo::Context>& cr)
{
  Glib::RefPtr<Gtk::StyleContext> style = m_pluginLabelBox->get_style_context();
  Gtk::Allocation allocation = m_pluginLabelBox->get_allocation();

  style->set_state(Gtk::STATE_FLAG_SELECTED);
  style->render_background(cr, 0, 0, allocation.get_width(), allocation.get_height());
  style->set_state(Gtk::STATE_FLAG_NORMAL);
  style->render_focus(cr, 0, 0, allocation.get_width(), allocation.get_height());

  return false;
}

PreferencesDialog::PreferencesDialog()
    : m_pageStoreCols(),
      m_currentPage(0)
{
  Glib::RefPtr<Gtk::Builder> builder = Gtk::Builder::create_from_file(
      Util::build_glade_path("preferences.glade"));
  Gtk::Widget *hbox;

  builder->get_widget("preferences_hbox", hbox);
  builder->get_widget("plugin_tree", m_pluginTree);
  builder->get_widget("plugin_label", m_pluginLabel);
  builder->get_widget("plugin_label_box", m_pluginLabelBox);
  builder->get_widget("plugin_container", m_pluginContainer);

  m_pluginLabelBox->set_app_paintable(true);
  m_pluginLabelBox->signal_draw().connect(sigc::mem_fun(*this, &PreferencesDialog::header_draw), false);
  m_pluginLabelBox->set_state(Gtk::STATE_SELECTED);

  m_refPageStore = Gtk::ListStore::create(m_pageStoreCols);
  m_refSettings = Util::gsettings_create();
  Gtk::TreeModel::iterator firstIter = populate_page_store();

  m_pluginTree->set_model(m_refPageStore);
  m_pluginTree->append_column("Name", m_pageStoreCols.m_col_name);
  m_pluginTree->get_selection()->signal_changed().connect(sigc::mem_fun(*this, &PreferencesDialog::page_selection_changed));
  m_pluginTree->get_selection()->select(firstIter);

  set_title("xbmcremote Preferences");
  set_default_size(600, 450);
  get_content_area()->add(*hbox);

  add_button(Gtk::Stock::CLOSE, Gtk::RESPONSE_CLOSE);
}

Gtk::TreeModel::iterator PreferencesDialog::add_page(PreferencesPage ppid, const Glib::ustring &name)
{
  Gtk::TreeModel::iterator iter = m_refPageStore->append();
  Gtk::TreeModel::Row row = *iter;

  row[m_pageStoreCols.m_col_id] = ppid;
  row[m_pageStoreCols.m_col_name] = name;

  return iter;
}

Gtk::TreeModel::iterator PreferencesDialog::populate_page_store()
{
  Gtk::TreeModel::iterator firstIter = add_page(PP_CONNECTION, "Connection");
  add_page(PP_DESKTOP, "Desktop");
  return firstIter;
}

}
