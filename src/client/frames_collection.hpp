
#ifndef _TMS_CLIENT__FRAMES_COLLECTION_HPP_
#define _TMS_CLIENT__FRAMES_COLLECTION_HPP_
namespace tms {

namespace common {
namespace widget {
class DlgCheckColumn;
}
}

namespace client {
class DlgContactPerson;
class CompaniesFrame;
class DlgUser;
class DlgSubject;
class DlgIncoming;
class DlgAddressee;
class DlgCompany ;
class FrmGrid;
class FramesCollection {
public:
  static DlgContactPerson *dlg_contact_person;
  static DlgSubject *dlg_subject;
  static DlgAddressee *dlg_addressee;
  static DlgUser *dlg_user;
  static DlgCompany *dlg_company;
  static DlgIncoming *dlg_incoming;
  static CompaniesFrame *companies_frame;
  static FrmGrid *frm_grid;
};
}
}
#endif // _TMS_CLIENT__FRAMES_COLLECTION_HPP_
