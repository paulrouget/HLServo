use std::cell::Cell;

#[no_mangle]
pub extern fn foobar() -> i32 {
    let o = Cell::new(2);
    o.get()
}
