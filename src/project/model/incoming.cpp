
#include "incoming.hpp"
// common
#include <contraption/field/simple_field.hpp>
#include <contraption/field/proxy_field.hpp>
// project
#include <project/model/company.hpp>
#include <project/model/contact_person.hpp>
using namespace tms::project::model;
using namespace tms::common::contraption;
using namespace std;

void Incoming::Initialize() {
  vector<Field*> ret;
  ret.push_back(new SimpleFieldT<string>("ID"));
  ret.push_back(new SimpleFieldT<string>("time_in"));
  ret.push_back(new SimpleFieldT<string>("type_in"));
  ret.push_back(new SimpleFieldT<string>("time_out"));
  ret.push_back(new SimpleFieldT<string>("topic"));
  

  ret.push_back(new SimpleFieldT<int>("company_id"));
  HasManyField* company_field 
    = new HasManyField("company", 
		       boost::ref(*Company::GetModel()),
		       boost::ref(*Incoming::GetModel()),
		       _id_column = "id",
		       _other_id_column = "company_id");
  ret.push_back(company_field);
  ret.push_back(new ProxyField<std::string>("company_name",
					    company_field,
					    "short_name"));

  ret.push_back(new SimpleFieldT<int>("contact_person_id"));
  HasManyField* contact_person_field 
    = new HasManyField("contact_person", 
		       boost::ref(*ContactPerson::GetModel()),
		       boost::ref(*Incoming::GetModel()),
		       _id_column = "id",
		       _other_id_column = "contact_person_id");
  ret.push_back(contact_person_field);
  ret.push_back(new ProxyField<std::string>("contact_person_name",
					    contact_person_field,
					    "name"));

  InitFields(ret);
}

