
#include "contact_person.hpp"
// common
#include <contraption/field/simple_field.hpp>
#include <contraption/field/has_many_field.hpp>
#include <contraption/field/proxy_field.hpp>
// project
#include <project/model/company.hpp>
using namespace tms::project::model;
using namespace tms::common::contraption;
using namespace std;

void ContactPerson::Initialize() {
  vector<Field*> ret;
  ret.push_back(new SimpleFieldT<string>("name"));
  ret.push_back(new SimpleFieldT<string>("role"));
  ret.push_back(new SimpleFieldT<string>("code"));
  ret.push_back(new SimpleFieldT<string>("email"));
  ret.push_back(new SimpleFieldT<string>("note"));
  ret.push_back(new SimpleFieldT<string>("phone"));
  ret.push_back(new SimpleFieldT<string>("fax"));
  ret.push_back(new SimpleFieldT<int>("company_id"));
  HasManyField* company_field 
    = new HasManyField("company", 
		       boost::ref(*Company::GetModel()),
		       boost::ref(*ContactPerson::GetModel()),
		       _id_column = "id",
		       _other_id_column = "company_id");
  ret.push_back(company_field);
  ret.push_back(new ProxyField<std::string>("company_name",
					    company_field,
					    "short_name"));

  InitFields(ret);
}

