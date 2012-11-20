
#include "company.hpp"
// common
#include <contraption/field/simple_field.hpp>
#include <contraption/field/has_many_field.hpp>
#include <contraption/field/proxy_field.hpp>
#include <model/user.hpp>
// project
#include <project/model/contact_person.hpp>
// oops
#include <iostream>
using namespace tms::project::model;
using namespace tms::common::contraption;
using namespace std;

void Company::Initialize() {
  vector<Field*> ret;
  ret.push_back(new StringField("ID"));
  ret.push_back(new StringField("full_name"));
  ret.push_back(new StringField("short_name"));
  ret.push_back(new IntField("gen_dir_id"));

  Company::GetModel();
  ContactPerson::GetModel();

  HasManyField* gen_dir_field 
    = new HasManyField("gen_dir", 
		       boost::ref(*Company::GetModel()),
		       boost::ref(*ContactPerson::GetModel()),
		       _id_column = "id",
		       _other_id_column = "gen_dir_id");
  ret.push_back(gen_dir_field);
  ret.push_back(new ProxyField<std::string>("gen_dir_name",
					    gen_dir_field,
					    "name"));
  ret.push_back(new IntField("glavbuh_id"));
  HasManyField* glavbuh_field 
    = new HasManyField("glavbuh", 
		       boost::ref(*Company::GetModel()),
		       boost::ref(*ContactPerson::GetModel()),
		       _id_column = "id",
		       _other_id_column = "glavbuh_id");
  ret.push_back(glavbuh_field);
  ret.push_back(new ProxyField<std::string>("glavbuh_name",
					    gen_dir_field,
					    "name"));

  ret.push_back(new IntField("manager_id"));
  HasManyField* manager_field 
    = new HasManyField("mamager", 
		       boost::ref(*Company::GetModel()),
		       boost::ref(*ContactPerson::GetModel()),
		       _id_column = "id",
		       _other_id_column = "manager_id");
  ret.push_back(manager_field);
  ret.push_back(new ProxyField<std::string>("manager_name",
					    gen_dir_field,
					    "name"));
  ret.push_back(new StringField("yur_addr"));
  ret.push_back(new StringField("fact_addr"));
  ret.push_back(new StringField("bank"));
  ret.push_back(new StringField("lits_schet"));
  ret.push_back(new StringField("gruppa"));
  ret.push_back(new StringField("bank_city"));
  ret.push_back(new StringField("cor_schet"));
  ret.push_back(new StringField("inn"));
  ret.push_back(new StringField("bik"));
  ret.push_back(new StringField("okonh"));
  ret.push_back(new StringField("ogrn"));
  ret.push_back(new StringField("comment"));
  ret.push_back(new StringField("phone"));
  ret.push_back(new StringField("mail"));
  InitFields(ret);
}

