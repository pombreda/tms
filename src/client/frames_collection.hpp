#ifndef _TMS_CLIENT__FRAMES_COLLECTION_HPP_
#define _TMS_CLIENT__FRAMES_COLLECTION_HPP_
namespace tms {
namespace client {
class DlgContactPerson;
class CompaniesFrame;
class DlgUser;
class IncomingsFrame;
class FrmGrid;
class DlgCheckColumn;
class FramesCollection {
public:
  static DlgContactPerson *contact_persons_frame;
  static DlgUser *dlg_user;
  static DlgCheckColumn *dlg_check_column;
  static IncomingsFrame *incomings_frame;
  static CompaniesFrame *companies_frame;
  static FrmGrid *frm_grid;
};
}
}
#endif // _TMS_CLIENT__FRAMES_COLLECTION_HPP_
