integer function test_ygg_input_1() result(r)
  use YggInterface
  type(yggcomm) :: sComm, rComm
  type(yggdtype) :: sDtype
  character(len=20) :: data_recv, data_send
  integer :: data_recv_len, data_send_len, i
  r = 1
  i = 0
  data_send = "Test message"
  data_send_len = 12
  data_recv_len = 20
  i = i + 1
  write (*, *) "HERE", i
  call dummy_function()
  i = i + 1
  write (*, *) "HERE", i
  sDtype = create_dtype_from_schema('{"type": "string"}', .false.)
  i = i + 1
  write (*, *) "HERE", i
  ! sComm = init_comm("test_name", 0, 1, sDtype, 131072)
  ! i = i + 1
  ! write (*, *) "HERE", i
  ! rComm = ygg_input("test_name")
  ! i = i + 1
  ! write (*, *) "HERE", i
  ! if (.NOT.c_associated(rComm%comm)) then
  !    write(*,*) "error in comm init"
  !    return
  ! end if
  ! i = i + 1
  ! write (*, *) "HERE", i
  ! if (.NOT.ygg_send(sComm, data_send, data_send_len)) then
  !    write(*,*) "send failed"
  !    return
  ! end if
  ! i = i + 1
  ! write (*, *) "HERE", i
  ! if (.NOT.ygg_recv(rComm, data_recv, data_recv_len)) then
  !    write(*,*) "recv failed"
  !    return
  ! end if
  ! i = i + 1
  ! write (*, *) "HERE", i
  ! if (data_recv_len.NE.data_send_len) then
  !    write(*,*) "data size not equal", data_send_len, data_recv_len
  !    return
  ! end if
  ! i = i + 1
  ! write (*, *) "HERE", i
  ! if (data_recv.NE.data_send) then
  !    write(*,*) "data not equal", data_send, data_recv
  !    return
  ! end if
  ! i = i + 1
  ! write (*, *) "HERE", i
  r = 0
end function test_ygg_input_1
