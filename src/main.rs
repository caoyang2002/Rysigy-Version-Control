use std::arch::aarch64::vuqadds_s32;

fn closure_fn<F>(func: F) // 不可变推荐使用
    where
        F: Fn()
{
    func();
    func();
}

fn closure_fn_mut<F>(mut func: F)
    where
        F: FnMut()
{
    func();
    func();
}

fn closure_fn_once<F>(func: F)
    where
        F: FnOnce()
{
    func();
}
fn main(){
    // 不可变引用只能传一种
    let s1 = String::from("1111");
    closure_fn(||println!("{}",s1) );

    // 可变引用
    let s1 = String::from("1111");
    closure_fn_mut(||println!("{}",s1) );

    let mut s2 = String::from("2222");
    closure_fn_mut(|| {
        s2.push_str("str");
        println!("s2 is {}",s2)
    });
    println!("s2 is {}",s2);
    // s2 is 2222strstr



    println!("所有权转移");
    // FnOnce 所有权转移
    let s1 = String::from("1111");
    closure_fn_once(||println!("{}",s1) );
    // 1111

    let mut s2 = String::from("2222");
    closure_fn_once(|| {
        s2.push_str("str");
        println!("s2 is {}",s2)
    });
    println!("s2 is {}",s2);
    // s2 is 2222str
    // s2 is 2222str

    let s3 = "string".to_owned();
    closure_fn_once(move || println!("s3 is {s3}"));
    // println!("{s3}"); //  所有权已经转移了：value borrowed here after move
}