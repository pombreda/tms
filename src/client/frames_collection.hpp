#ifndef _TMS_CLIENT__FRAMES_COLLECTION_HPP_
#define _TMS_CLIENT__FRAMES_COLLECTION_HPP_
namespace tms {
namespace client {
class ContactPersonsFrame;
class CompaniesFrame;
class UsersFrame;
class IncomingsFrame;
class GridFrame;
class FramesCollection {
public:
  static ContactPersonsFrame *contact_persons_frame;
  static UsersFrame *users_frame;
  static IncomingsFrame *incomings_frame;
  static CompaniesFrame *companies_frame;
  static GridFrame *grid_frame;
};
}
}
#endif // _TMS_CLIENT__FRAMES_COLLECTION_HPP_
