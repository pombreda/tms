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
class DlgIncoming;
class IncomingsFrame;
class FrmGrid;
class FramesCollection {
public:
  static DlgContactPerson *contact_persons_frame;
  static DlgUser *dlg_user;
  static IncomingsFrame *incomings_frame;
  static DlgIncoming *dlg_incoming;
  static CompaniesFrame *companies_frame;
  static FrmGrid *frm_grid;
};
}
}
#endif // _TMS_CLIENT__FRAMES_COLLECTION_HPP_
