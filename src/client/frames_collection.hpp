#ifndef _TMS_CLIENT__FRAMES_COLLECTION_HPP_
#define _TMS_CLIENT__FRAMES_COLLECTION_HPP_
namespace tms {
namespace client {
class ContactPersonsFrame;
class CompaniesFrame;
class DlgUser;
class IncomingsFrame;
class FrmGrid;
class FramesCollection {
public:
  static ContactPersonsFrame *contact_persons_frame;
  static DlgUser *dlg_user;
  static IncomingsFrame *incomings_frame;
  static CompaniesFrame *companies_frame;
  static FrmGrid *frm_grid;
};
}
}
#endif // _TMS_CLIENT__FRAMES_COLLECTION_HPP_
