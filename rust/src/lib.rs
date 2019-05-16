use rust_webvr as webvr;

#[no_mangle]
pub extern fn foobar() -> i32 {
    let mut vr = webvr::VRServiceManager::new();
    vr.register_defaults();
    vr.register_mock();
    vr.initialize_services();
    0
}
