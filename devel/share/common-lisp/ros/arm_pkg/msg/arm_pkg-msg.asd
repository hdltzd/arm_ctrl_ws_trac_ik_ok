
(cl:in-package :asdf)

(defsystem "arm_pkg-msg"
  :depends-on (:roslisp-msg-protocol :roslisp-utils )
  :components ((:file "_package")
    (:file "handle_info" :depends-on ("_package_handle_info"))
    (:file "_package_handle_info" :depends-on ("_package"))
  ))