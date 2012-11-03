#ifndef _COMMON_CONTRAPTION__MODEL_WRAPPER_HPP_
#define _COMMON_CONTRAPTION__MODEL_WRAPPER_HPP_
#include <common/contraption/model.hpp>
#include <boost/unordered_map.hpp>
#include <iostream> // oops
using namespace std;

namespace tms {
namespace common {
namespace contraption {
template<class T>
class ModelWrapper;
}
}
}

namespace tms {
namespace common {
namespace contraption {
// only one backend per lunch for now
template< class T >
class ModelWrapper : public Model {
  typedef T* TP;
  public:
    static ModelP GetModelLink(ModelBackendP backend) {
      cerr << "before init" << endl;
      ModelP &model_ = model_map_[&*backend];
      TP model = model_ ? dynamic_cast<T*>(&*(model_)) : 0;
      cerr << "after init" << endl;
      if (!model || true) { //oops!
        model_.reset(new T(backend));
      }
      return model_;
    }

    static ModelP GetModelLink(ModelBackend *backend) {
      cerr << "before init" << endl;
      ModelP &model_ = model_map_[&*backend];
      TP model = model_ ? dynamic_cast<T*>(&*(model_)) : 0;
      cerr << "after init" << endl;
      if (!model || true) {
        model_.reset(new T(backend));
      }
      return model_;
    }

    static ModelP GetModel(ModelBackendP backend) {
      ModelP model_ = GetModelLink(backend);
      TP model = dynamic_cast<T*>(&*model_);
      if (!model->initialized_) {
        model->initialized_ = true;
        model->Initialize();
      }
      return model_;
    }
    static ModelP GetModel(ModelBackend *backend) {
      ModelP model_ = GetModelLink(backend);
      TP model = dynamic_cast<T*>(&*model_);
      if (!model->initialized_) {
        model->initialized_ = true;
        model->Initialize();
      }
      return model_;
    }
    friend void boost::intrusive_ptr_add_ref(Model* model);
    friend void boost::intrusive_ptr_release(Model* model);
    protected:
      virtual void Initialize() = 0;
      ModelWrapper(ModelBackendP backend) : Model(backend), initialized_(false) {}
      ModelWrapper(ModelBackend *backend) : Model(backend), initialized_(false) {}
  private:
    static boost::unordered_map<ModelBackend*, ModelP> model_map_;
    bool initialized_;
};
template<class T>
boost::unordered_map<ModelBackend*, ModelP> ModelWrapper<T>::model_map_;

}
}
}


#endif // _COMMON_CONTRAPTION__MODEL_WRAPPER_HPP_
